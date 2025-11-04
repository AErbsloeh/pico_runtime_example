from logging import getLogger, Logger
from serial import Serial, STOPBITS_ONE, EIGHTBITS, PARITY_NONE
from serial.tools import list_ports
from time import sleep


class ProcessInteractionPico:
    __logger: Logger
    __device: Serial
    __BYTES_HEAD: int = 1
    __BYTES_DATA: int = 2

    def __init__(self, device: Serial, num_bytes_head: int=1, num_bytes_data: int=2) -> None:
        """Class for interacting with the USB serial devices
        :param device:  Class with properties of the Serial device
        :param num_bytes_head: Number of bytes head, implemented on Pico
        :param num_bytes_data: Number of bytes data, implemented on Pico
        """
        self.__logger = getLogger(__name__)
        self.__BYTES_HEAD = num_bytes_head
        self.__BYTES_DATA = num_bytes_data
        self.__device = device

    @property
    def total_num_bytes(self) -> int:
        """Returning the total number of bytes for each transmission"""
        return self.__BYTES_DATA + self.__BYTES_HEAD

    def is_open(self) -> bool:
        """Return True if the device is open, False otherwise"""
        return self.__device.is_open

    def read(self, no_bytes: int):
        """Read content from device"""
        return self.__device.read(no_bytes)

    def write_wofb(self, data: bytes) -> None:
        """Write content to device without feedback"""
        self.__device.write(data)

    def write_wfb(self, data: bytes):
        """Write all information to device (specific bytes)"""
        self.__device.write(data)
        return self.__device.read(len(data))

    def write_wfb_lf(self, data: bytes) -> bytes:
        """Write all information to device (unlimited bytes until LF)"""
        self.__device.write(data)
        return self.__device.read_until()

    def write_wfb_hex(self, head: int, data: int) -> bytes:
        """Write content to FPGA/MCU for specific custom-made task"""
        transmit_byte = head.to_bytes(self.__BYTES_HEAD, 'little')
        transmit_byte += data.to_bytes(self.__BYTES_DATA, 'big')
        self.__device.write(transmit_byte)
        return self.__device.read(len(transmit_byte))

    def open(self) -> None:
        """Starting a connection to device"""
        if self.__device.is_open:
            self.__device.close()
        self.__device.open()

    def close(self) -> None:
        """Closing a connection to device"""
        self.__device.close()


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
                    inter_byte_timeout=timeout
                )
            )
            if self.is_com_port_active:
                self.__device.close()
            self.__device.open()

    @property
    def __read_usb_properties(self) -> list:
        """Reading the USB properties of all devices device"""
        return [{"com": ps.device, "pid": ps.pid, "vid": ps.vid} for ps in list_ports.comports()]

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
        ret = self.__device.write_wfb(bytes([0x00, 0x00, 0x00]))
        return str(ret)

    def do_reset(self) -> None:
        """Performing a Software Reset on the Platform"""
        self.__device.write_wofb(bytes([0x01, 0x00, 0x00]))
        sleep(4)

    def get_system_state(self) -> str:
        """Retuning the System State of the System"""
        ret = self.__device.write_wfb(bytes([0x02, 0x00, 0x00]))
        return str(ret)

    def get_runtime(self) -> str:
        """"""
        ret = self.__device.write_wfb(bytes([0x03, 0x00, 0x00]))
        return str(ret)

    def enable_led(self):
        """Changing the state of the LED with enabling it"""
        self.__device.write_wofb(bytes([0x04, 0x00, 0x00]))

    def disable_led(self):
        """Changing the state of the LED with disabling it"""
        self.__device.write_wofb(bytes([0x05, 0x00, 0x00]))

    def toogle_led(self):
        """Changing the state of the LED with toggling it"""
        self.__device.write_wofb(bytes([0x06, 0x00, 0x00]))
