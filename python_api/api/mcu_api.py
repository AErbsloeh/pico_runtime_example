from dataclasses import dataclass
from logging import getLogger, Logger
from time import sleep

from api.interface import (
    get_comport_name,
    InterfaceSerialUSB
)
from api.lsl import ThreadLSL
from api.mcu_conv import (
    _convert_pin_state,
    _convert_system_state
)


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
        firmware:   String with firmware version on board
    """
    pins: str
    system: str
    runtime: float
    clock: int
    firmware: str


class DeviceAPI:
    __device: InterfaceSerialUSB
    __logger: Logger
    __num_bytes_runtime: int = 9
    __num_bytes_data: int = 14
    __sampling_rate: float = 4.
    # Pico 2 specific USB characteristics
    __usb_vid: int = 0x2E8A
    __usb_pid: int = 0x0009
    __threads: ThreadLSL

    def __init__(self, com_name: str="AUTOCOM", timeout: float=1.0) -> None:
        """Init. of the device with name and baudrate of the device
        :param com_name:    String with the serial port name of the used device
        :param timeout:     Floating value with timeout for the communication
        """
        self.__logger = getLogger(__name__)
        self.__threads = ThreadLSL()
        self.__device = InterfaceSerialUSB(
            com_name=com_name if com_name != "AUTOCOM" else get_comport_name(usb_vid=self.__usb_vid, usb_pid=self.__usb_pid),
            baud=115200,
            num_bytes_head=1,
            num_bytes_data=2,
            timeout=timeout,
        )
        if self.is_com_port_active:
            self.__device.close()
        self.__device.open()


    def __write_with_feedback(self, head: int, data: int, size: int=0) -> bytes:
        return self.__device.write_wfb(
            data=self.__device.convert(head, data),
            size=size
        )

    def __write_without_feedback(self, head: int, data: int) -> None:
        self.__device.write(
            data=self.__device.convert(head, data),
        )

    @property
    def total_num_bytes(self) -> int:
        """Returning the total number of bytes for each transmission"""
        return self.__device.total_num_bytes

    @property
    def is_com_port_active(self) -> bool:
        """Boolean for checking if serial communication is open and used"""
        return self.__device.is_open()

    @property
    def is_daq_running(self) -> bool:
        """Returning if DAQ is still running"""
        return self._get_system_state() == "DAQ" and self.__threads.is_alive

    def open(self) -> None:
        """Opening the serial communication between API and device"""
        self.__device.open()

    def close(self) -> None:
        """Closing the serial communication between API and device"""
        self.__device.close()

    def do_reset(self) -> None:
        """Performing a Software Reset on the Platform"""
        if self.__threads.is_alive:
            self.__threads.stop()
            self.stop_daq()
        self.__write_without_feedback(1, 0)
        sleep(4)

    def echo(self, data: str) -> str:
        """Sending some characters to the device and returning the result
        :param data:    String with the data to be sent
        :return:        String with returned data from DAQ
        """
        do_padding = len(data) % self.__device.num_bytes == 1
        val = bytes()
        for chunk in self.__device.serialize_string(data, do_padding):
            ret = self.__write_with_feedback(0, chunk)
            val += ret[1:]
            if ret[0] != 0x00:
                raise ValueError
        return self.__device.deserialize_string(val, do_padding)

    def _get_system_clock_khz(self) -> int:
        """Returning the system clock of the device in kHz"""
        ret = self.__write_with_feedback(2, 0)
        if ret[0] != 0x02:
            raise ValueError
        return 10 * int.from_bytes(ret[1:], byteorder='little', signed=False)

    def _get_system_state(self) -> str:
        """Retuning the System State"""
        ret = self.__write_with_feedback(3, 0)[-1]
        return _convert_system_state(ret)

    def _get_pin_state(self) -> str:
        """Retuning the Pin States"""
        ret = self.__write_with_feedback(4, 0)[-1]
        return _convert_pin_state(ret)

    def _get_runtime_sec(self) -> float:
        """Returning the execution runtime of the device after last reset
        :return:    Float value with runtime in seconds
        """
        ret = self.__write_with_feedback(5, 0, size=9)
        if ret[0] != 0x05:
            raise ValueError
        return 1e-6 * int.from_bytes(ret[1:], byteorder='little', signed=False)

    def _get_firmware_version(self) -> str:
        """Returning the firmware version of the device
        :return:    String with firmware version
        """
        ret = self.__write_with_feedback(6, 0)
        if ret[0] != 0x06:
            raise ValueError
        return f"{ret[1]}.{ret[2]}"

    def get_state(self) -> SystemState:
        """Returning the state of the system
        :return:    Class SystemState with information about pin state, system state and actual runtime of the system
        """
        return SystemState(
            pins=self._get_pin_state(),
            system=self._get_system_state(),
            runtime=self._get_runtime_sec(),
            clock=self._get_system_clock_khz(),
            firmware=self._get_firmware_version(),
        )

    def enable_led(self) -> None:
        """Changing the state of the LED with enabling it
        :return:        None
        """
        self.__write_without_feedback(7, 0)

    def disable_led(self) -> None:
        """Changing the state of the LED with disabling it
        :return:        None
        """
        self.__write_without_feedback(8, 0)

    def toggle_led(self) -> None:
        """Changing the state of the LED with toggling it
        :return:        None
        """
        self.__write_without_feedback(9, 0)

    @property
    def _thread_process_sample_in_lsl(self) -> list:
        """Indicating the indices from the daq sample list for further processing/saving (without timestamp)"""
        return [0, 1, 2]

    def _thread_prepare_daq_for_lsl(self) -> list:
        """Entpacken der Informationen aus dem USB Protokoll (siehe C-Datei: src/daq_sample.c in der Firmware)"""
        data = self.__device.read(15)
        if not len(data):
            return list()
        else:
            if not data[0] == 0xA0 or not data[-1] == 0xFF:
                return list()
            else:
                return [
                    int.from_bytes(data[2:10], byteorder='little', signed=False),   # Runtime MCU --> Goes to Timestamp
                    int(data[1]),                                                   # Index
                    int.from_bytes(data[10:12], byteorder='little', signed=False),  # uint16_t Data - CH0
                    int.from_bytes(data[12:14], byteorder='little', signed=False)   # uint16_t Data - CH1
                ]

    def start_daq(self, do_plot: bool=False, window_sec: float= 30., track_util: bool=False, folder_name: str="data") -> None:
        """Changing the state of the DAQ with starting it
        :param do_plot:     True to plot the data in real-time
        :param window_sec:  Floating value with window length [in seconds] for live plotting
        :param track_util:  If true, the utilization (CPU / RAM) of the host computer will be tracked during recording session
        :param folder_name: String with folder name to save data in project folder
        :return: None
        """
        path2data = get_path_to_project(new_folder=folder_name)
        self.__threads.register(func=self.__threads._thread_stream_data, args=(0, 'data', self._thread_prepare_daq_for_lsl, 3, self.__sampling_rate))
        self.__threads.register(func=self.__threads._thread_record_stream, args=(1, 'data', path2data, self._thread_process_sample_in_lsl))
        if track_util:
            self.__threads.register(func=self.__threads._thread_stream_util, args=(2, 'util', 2.))
            self.__threads.register(func=self.__threads._thread_record_stream, args=(3, 'util', path2data))
        if do_plot:
            self.__threads.register(func=self.__threads._thread_plot_stream, args=(4 if track_util else 3, 'data', window_sec, self._thread_process_sample_in_lsl))

        self.__threads.start()
        self.__write_without_feedback(10, 0)

    def stop_daq(self) -> None:
        """Changing the state of the DAQ with stopping it"""
        self.__threads.stop()
        self.__write_without_feedback(11, 0)

    def wait_daq(self, time_sec: float) -> None:
        """Waiting Routine"""
        self.__threads.wait_for_seconds(time_sec)

    def update_daq_sampling_rate(self, sampling_rate: float) -> None:
        """Updating the sampling rate of the DAQ
        :param sampling_rate:   Float with sampling rate [Hz]
        """
        sampling_limits = [0, 2**16-1]
        if sampling_rate < sampling_limits[0]:
            raise ValueError(f"Sampling rate cannot be smaller than {sampling_limits[0]}")
        if sampling_rate > 10e3:
            raise ValueError(f"Sampling rate cannot be greater than {sampling_limits[1]}")

        self.__sampling_rate = sampling_rate
        self.__write_without_feedback(12, int(sampling_rate))
