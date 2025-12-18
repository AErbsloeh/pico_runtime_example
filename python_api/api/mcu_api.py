from dataclasses import dataclass
from logging import getLogger, Logger
from time import sleep
from serial import Serial, STOPBITS_ONE, EIGHTBITS, PARITY_NONE
from serial.tools import list_ports
from threading import Thread, Event

from api.lsl import start_stream_data, start_stream_utilization, record_stream
from api.mcu_conv import _convert_pin_state, _convert_system_state
from api.interface import InterfaceSerialUSB


def get_path_to_project(new_folder: str='', max_levels: int=5) -> str:
    """Function for getting the path to find the project folder structure in application.
    :param new_folder:  New folder path
    :param max_levels:  Max number of levels to get-out for finding pyproject.toml
    :return:            String with absolute path to start the project structure
    """
    from pathlib import Path
    cwd = Path(".").absolute()
    current = cwd

    def is_project_root(p):
        return (p / "pyproject.toml").exists()

    for _ in range(max_levels):
        if is_project_root(current):
            return str(current / new_folder)
        current = current.parent

    if is_project_root(current):
        return str(current / new_folder)
    return str(cwd)


@dataclass(frozen=True)
class SystemState:
    """Dataclass for handling the system state of the device
    Attributes:
        pins:       String with enabled GPIO pins (selection from MCU)
        system:     String with actual system state
        runtime:    Float with actual execution runtime after last reset [sec.]
        clock:      Integer with System Clock [kHz]
    """
    pins: str
    system: str
    runtime: float
    clock: int


class DeviceAPI:
    __device: InterfaceSerialUSB
    __logger: Logger
    __num_bytes_runtime: int = 9
    __num_bytes_data: int = 14
    # Pico 2 specific USB characteristics
    __usb_vid: int = 0x2E8A
    __usb_pid: int = 0x0009
    __lsl_events: Event = Event()
    __lsl_threads: list[Thread] = list()

    def __init__(self, com_name: str="AUTOCOM", timeout: float=1.0, baud: int=115200) -> None:
        """Init. of the device with name and baudrate of the device
        :param com_name:    String with the serial port name of the used device
        :param timeout:     Floating value with timeout for the communication
        :param baud:        Baud rate of the communication between API and device [default: 115.200]
        """
        self.__logger = getLogger(__name__)
        self.__device = InterfaceSerialUSB(
            num_bytes_head=1,
            num_bytes_data=2,
            device=Serial(
                port=com_name if com_name != "AUTOCOM" else self.scan_com_name[0],
                baudrate=baud,
                parity=PARITY_NONE,
                stopbits=STOPBITS_ONE,
                bytesize=EIGHTBITS,
                inter_byte_timeout=timeout,
                xonxoff=False,
                rtscts=False,
                dsrdtr=False,
                timeout=0.5
            )
        )
        if self.is_com_port_active:
            self.__device.close()
        self.__device.open()

    @property
    def __read_usb_properties(self) -> list:
        return [{"com": ps.device, "pid": ps.pid, "vid": ps.vid} for ps in list_ports.comports()]

    def __write_wfb(self, head: int, data: int, size: int=0) -> bytes:
        return self.__device.write_wfb(
            data=self.__device.convert(head, data),
            size=size
        )

    def __write_wofb(self, head: int, data: int, size: int=0) -> None:
        self.__device.write_wofb(
            data=self.__device.convert(head, data),
        )

    @property
    def scan_com_name(self) -> list:
        """Returning the COM Port name of the addressable devices
        :return: List of COM Port names with matched VIP und PID properties of the USB-COMs
        """
        available_coms = list_ports.comports()
        list_right_com = [port.device for port in available_coms if
                          port.vid == self.__usb_vid and port.pid == self.__usb_pid]
        if len(list_right_com) == 0:
            raise ConnectionError(f"No COM Port with right USB found - Please adapt the VID and PID values from "
                                  f"available COM ports: {self.__read_usb_properties}")
        self.__logger.debug(f"Found {len(list_right_com)} COM ports available")
        return list_right_com

    @property
    def total_num_bytes(self) -> int:
        """Returning the total number of bytes for each transmission"""
        return self.__device.total_num_bytes

    @property
    def is_com_port_active(self) -> bool:
        """Boolean for checking if serial communication is open and used"""
        return self.__device.is_open()

    @property
    def is_daq_active(self) -> bool:
        """Returning if DAQ is still running"""
        return self._get_system_state() == "DAQ" and self.__lsl_events.is_set()

    def open(self) -> None:
        """Opening the serial communication between API and device"""
        self.__device.open()

    def close(self) -> None:
        """Closing the serial communication between API and device"""
        self.__device.close()

    def do_reset(self) -> None:
        """Performing a Software Reset on the Platform"""
        if len(self.__lsl_threads):
            self.stop_daq()

        self.__write_wofb(1, 0)
        sleep(4)

    def echo(self, data: str) -> str:
        """Sending some characters to the device and returning the result
        :param data:    String with the data to be sent
        :return:        String with returned data from DAQ
        """
        do_padding = len(data) % self.__device.num_bytes == 1
        val = bytes()
        for chunk in self.__device.serialize_string(data, do_padding):
            ret = self.__write_wfb(0, chunk)
            val += ret[1:]
            if ret[0] != 0x00:
                raise ValueError
        return self.__device.deserialize_string(val, do_padding)

    def _get_system_clock_khz(self) -> int:
        """Returning the system clock of the device in kHz"""
        ret = self.__write_wfb(2, 0)
        if ret[0] != 0x02:
            raise ValueError
        return 10 * int.from_bytes(ret[1:], byteorder='little', signed=False)

    def _get_system_state(self) -> str:
        """Retuning the System State"""
        ret = self.__write_wfb(3, 0)[-1]
        return _convert_system_state(ret)

    def _get_pin_state(self) -> str:
        """Retuning the Pin States"""
        ret = self.__write_wfb(4, 0)[-1]
        return _convert_pin_state(ret)

    def _get_runtime_sec(self) -> float:
        """Returning the execution runtime of the device after last reset
        :return:    Float value with runtime in seconds
        """
        ret = self.__write_wfb(5, 0, size=9)
        if ret[0] != 0x05:
            raise ValueError
        return 1e-6 * int.from_bytes(ret[1:], byteorder='little', signed=False)

    def get_state(self) -> SystemState:
        """Returning the state of the system
        :return:    Class SystemState with information about pin state, system state and actual runtime of the system
        """
        return SystemState(
            pins=self._get_pin_state(),
            system=self._get_system_state(),
            runtime=self._get_runtime_sec(),
            clock=self._get_system_clock_khz()
        )

    def enable_led(self) -> None:
        """Changing the state of the LED with enabling it
        :return:        None
        """
        self.__write_wofb(6, 0)

    def disable_led(self) -> None:
        """Changing the state of the LED with disabling it
        :return:        None
        """
        self.__write_wofb(7, 0)

    def toggle_led(self) -> None:
        """Changing the state of the LED with toggling it
        :return:        None
        """
        self.__write_wofb(8, 0)

    @property
    def _thread_process_sample_in_lsl(self) -> list:
        """Indicating the indices from the daq sample list for further processing/saving"""
        return [1, -1]

    def _thread_prepare_daq_for_lsl(self) -> list:
        """Entpacken der Informationen aus dem USB Protokoll (siehe C-Datei: src/daq_sample.c in der Firmware)"""
        data = self.__device.read(14)
        if not len(data):
            return list()
        else:
            if not data[0] == 0xA0 or not data[-1] == 0xFF:
                return list()
            else:
                return [
                    int.from_bytes(data[2:10], byteorder='little', signed=False),   # Runtime MCU
                    int(data[1]),                                                   # Index
                    int(data[10]),                                                  # Channel Number
                    int.from_bytes(data[11:13], byteorder='little', signed=False)   # uint16_t Data
                ]

    def start_daq(self, track_util: bool=False, folder_name: str="data") -> None:
        """Changing the state of the DAQ with starting it
        :param track_util:  If true, the utilization (CPU / RAM) of the host computer will be tracked during recording session
        :param folder_name: String with folder name to save data in project folder
        :return: None
        """
        path2data = get_path_to_project(new_folder=folder_name)
        self.__lsl_threads = [Thread(target=start_stream_data, args=("data", 4, self._thread_prepare_daq_for_lsl, self.__lsl_events), daemon=True)]
        self.__lsl_threads.append(Thread(target=record_stream, args=("data", path2data, self.__lsl_events, self._thread_process_sample_in_lsl, 2)))
        if track_util:
            self.__lsl_threads.append(Thread(target=start_stream_utilization, args=("util", self.__lsl_events, 2.)))
            self.__lsl_threads.append(Thread(target=record_stream, args=("util", path2data, self.__lsl_events)))

        self.__lsl_events.set()
        for p in self.__lsl_threads:
            p.start()

        self.__write_wofb(9, 0)

    def stop_daq(self) -> None:
        """Changing the state of the DAQ with stopping it"""
        self.__lsl_events.clear()
        for p in self.__lsl_threads:
            p.join(timeout=1.)

        self.__write_wofb(10, 0)

    def update_daq_sampling_rate(self, sampling_rate: float) -> None:
        """Updating the sampling rate of the DAQ
        :param sampling_rate:   Float with sampling rate [Hz]
        """
        # TODO: Update func
        sampling_limits = [1e0, 10e3]
        if sampling_rate < sampling_limits[0]:
            raise ValueError(f"Sampling rate cannot be smaller than {sampling_limits[0]}")
        if sampling_rate > 10e3:
            raise ValueError(f"Sampling rate cannot be greater than {sampling_limits[1]}")

        digit_rate = int(1e6 / 256 / sampling_rate)
        self.__write_wofb(11, digit_rate)
