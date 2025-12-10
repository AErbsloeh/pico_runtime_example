from logging import getLogger, Logger
from time import sleep
from serial import Serial, STOPBITS_ONE, EIGHTBITS, PARITY_NONE
from serial.tools import list_ports
from multiprocessing import Process
from psutil import cpu_percent, virtual_memory
from pylsl import StreamInfo, StreamOutlet, cf_int32, cf_float32

from api.lsl import record_stream
from api.conversion import _convert_pin_state, _convert_system_state, SystemState
from api.serial import ProcessInteractionPico


class DeviceAPI:
    __device: ProcessInteractionPico
    __logger: Logger
    __num_bytes_runtime: int = 9
    __num_bytes_data: int = 14
    # Pico 2 specific USB characteristics
    __usb_vid: int = 0x2E8A
    __usb_pid: int = 0x0009
    _processes: list = list()

    def __init__(self, com_name: str="AUTOCOM", timeout: float=1.0, baud: int=115200) -> None:
        """Init. of the device with name and baudrate of the device
        :param com_name:    String with the serial port name of the used device
        :param timeout:     Floating value with timeout for the communication
        :param baud:        Baud rate of the communication between API and device [default: 115.200]
        """
        self.__logger = getLogger(__name__)
        self.__device = ProcessInteractionPico(
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
                dsrdtr=False
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
        """Returning the COM Port name of the addressable devices"""
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
    def is_com_port_active(self):
        """Boolean for checking if serial communication is open and used"""
        return self.__device.is_open()

    def open(self) -> None:
        """Opening the serial communication between API and device"""
        self.__device.open()

    def close(self) -> None:
        """Closing the serial communication between API and device"""
        self.__device.close()

    def do_reset(self) -> None:
        """Performing a Software Reset on the Platform"""
        if len(self._processes):
            self.stop_daq()

        self.__write_wofb(1, 0)
        sleep(4)

    def echo(self, data: str) -> str:
        """Sending some characters to the device and returning the result"""
        do_padding = len(data) % self.__device.num_bytes == 1
        val = bytes()
        for chunk in self.__device.serialize_string(data, do_padding):
            ret = self.__write_wfb(0, chunk)
            val += ret[1:]
            if ret[0] != 0x00:
                raise ValueError
        return self.__device.deserialize_string(val, do_padding)

    def _get_system_state(self) -> str:
        """Retuning the System State"""
        ret = self.__write_wfb(2, 0)[-1]
        return _convert_system_state(ret)

    def _get_pin_state(self) -> str:
        """Retuning the Pin States"""
        ret = self.__write_wfb(3, 0)[-1]
        return _convert_pin_state(ret)

    def _get_runtime_sec(self) -> float:
        """Returning the execution runtime of the device after last reset in seconds"""
        ret = self.__write_wfb(4, 0, size=9)
        if ret[0] != 0x04:
            raise ValueError
        return int.from_bytes(ret[1:], byteorder='little', signed=False) / 1e6

    def get_state(self) -> SystemState:
        """Returning the state of the system"""
        return SystemState(
            pins=self._get_pin_state(),
            system=self._get_system_state(),
            runtime=self._get_runtime_sec()
        )

    def enable_led(self) -> None:
        """Changing the state of the LED with enabling it"""
        self.__write_wofb(5, 0)

    def disable_led(self) -> None:
        """Changing the state of the LED with disabling it"""
        self.__write_wofb(6, 0)

    def toggle_led(self) -> None:
        """Changing the state of the LED with toggling it"""
        self.__write_wofb(7, 0)

    def _start_stream_data(self, name: str) -> None:
        config = StreamInfo(
            name=name,
            type='custom_daq',
            channel_count=4,
            nominal_srate=1.,
            channel_format=cf_int32,
            source_id=name + '_uid'
        )
        outlet = StreamOutlet(config)

        while True:
            data = self.__device.read(14)
            if not data[0] == 0xA0 or not data[-1] == 0xFF:
                continue
            outlet.push_sample([
                int.from_bytes(data[2:10], byteorder='little', signed=False),   # Runtime MCU
                int(data[1]),                                                   # Index
                int(data[10]),                                                  # Channel Number
                int.from_bytes(data[11:13], byteorder='little', signed=False)   # uint16_t Data
            ])

    @staticmethod
    def _start_stream_utilization(name: str, sampling_rate: float) -> None:
        config = StreamInfo(
            name=name,
            type='utilization',
            channel_count=2,
            nominal_srate=sampling_rate,
            channel_format=cf_float32,
            source_id=name + '_uid'
        )
        outlet = StreamOutlet(config)
        while True:
            outlet.push_sample([cpu_percent(interval=1 / sampling_rate), virtual_memory().percent])
            sleep(1 / sampling_rate)

    def start_daq(self, track_util: bool=False) -> None:
        """Changing the state of the DAQ with starting it"""
        #self._processes = [Process(target=, args=("data", ))]
        self._processes = [Process(target=record_stream, args=("data", "../data"))]
        #if track_util:
        #    self._processes.append(Process(target=self._start_stream_utilization, args=("util", 2.)))

        for p in self._processes:
            p.start()
        self.__write_wofb(8, 0)
        self._start_stream_data("data")

    def stop_daq(self) -> None:
        """Changing the state of the DAQ with stopping it"""
        self.__write_wofb(9, 0)
        for p in self._processes:
            p.terminate()

    def update_daq_sampling_rate(self, sampling_rate: float) -> None:
        """Updating the sampling rate of the DAQ"""
        sampling_limits = [1e0, 10e3]
        if sampling_rate < sampling_limits[0]:
            raise ValueError(f"Sampling rate cannot be smaller than {sampling_limits[0]}")
        if sampling_rate > 10e3:
            raise ValueError(f"Sampling rate cannot be greater than {sampling_limits[1]}")

        digit_rate = int(1e6 / 256 / sampling_rate)
        self.__write_wofb(10, digit_rate)
