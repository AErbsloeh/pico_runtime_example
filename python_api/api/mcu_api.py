from dataclasses import dataclass
from logging import getLogger, Logger
from time import sleep
from serial import Serial, STOPBITS_ONE, EIGHTBITS, PARITY_NONE
from serial.tools import list_ports
from api.serial import ProcessInteractionPico


def _convert_pin_state(state: int) -> str:
    if state == 0:
        return 'None'
    else:
        return 'LED_USER'


def _convert_system_state(state: int) -> str:
    state_name = ["NONE", "RESET", "INIT", "IDLE", "TEST", "ERROR"]
    return state_name[state]


@dataclass
class SystemState:
    """Dataclass for handling the system state of the device
    Attributes:
        pins:       String with enabled GPIO pins (selection from MCU)
        system:     String with actual system state
        runtime:    Float with actual execution runtime after last reset
    """
    pins: str
    system: str
    runtime: float


class DeviceAPI:
    __device: ProcessInteractionPico
    __logger: Logger
    __usb_vid: int = 0x2E8A # Pico2 specific device number
    __usb_pid: int = 0x0009 #

    def __init__(self, com_name: str="AUTOCOM", timeout: float=1.0, baud: int=115200, make_connection: bool=True) -> None:
        """Init. of the device with name and baudrate of the device
        :param com_name:    String with the serial port name of the used device
        :param timeout:     Floating value with timeout for the communication
        :param baud:        Baud rate of the communication between API and device [default: 115.200]
        """
        self.__logger = getLogger(__name__)
        if make_connection:
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
        return self.__device.is_open

    def open(self) -> None:
        """Opening the serial communication between API and device"""
        self.__device.open()

    def close(self) -> None:
        """Closing the serial communication between API and device"""
        self.__device.close()

    def echo(self, data: str) -> str:
        """Sending some characters to the device and returning the result"""
        padding = len(data) % self.__device.num_bytes == 1
        if padding:
            data += " "

        chunks = [int.from_bytes(data[i:i+2].encode('utf-8'), 'big') for i in range(0, len(data), 2)]
        val = bytes()
        for chunk in chunks:
            ret = self.__write_wfb(0, chunk)[1:]
            if ret[0] != 0x00:
                raise ValueError
            val += ret
        if padding:
            val = val[:-1]
        return val.decode('utf8')

    def do_reset(self) -> None:
        """Performing a Software Reset on the Platform"""
        self.__write_wfb(1, 0)
        sleep(4)

    def get_system_state(self) -> str:
        """Retuning the System State"""
        ret = self.__write_wfb(2, 0)[-1]
        return _convert_system_state(ret)

    def get_pin_state(self) -> str:
        """Retuning the Pin States"""
        ret = self.__write_wfb(3, 0)[-1]
        return _convert_pin_state(ret)

    def get_runtime_sec(self) -> float:
        """Returning the execution runtime of the device after last reset in seconds"""
        ret = self.__write_wfb(4, 0, size=9)
        return int.from_bytes(ret[1:], byteorder='little', signed=False) / 1e6

    def get_state(self) -> SystemState:
        """Returning the state of the system"""
        return SystemState(
            pins=self.get_pin_state(),
            system=self.get_system_state(),
            runtime=self.get_runtime_sec()
        )

    def enable_led(self):
        """Changing the state of the LED with enabling it"""
        self.__write_wofb(5, 0)

    def disable_led(self):
        """Changing the state of the LED with disabling it"""
        self.__write_wofb(6, 0)

    def toogle_led(self):
        """Changing the state of the LED with toggling it"""
        self.__write_wofb(7, 0)
