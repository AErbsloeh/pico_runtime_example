from logging import Logger, getLogger
from serial import Serial


class InterfaceSerialUSB:
    __logger: Logger
    __device: Serial
    __BYTES_HEAD: int
    __BYTES_DATA: int

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

    @property
    def num_bytes(self) -> int:
        """Returning the number of data bytes in each transmission"""
        return self.__BYTES_DATA

    def convert(self, head: int, data: int) -> bytes:
        """"""
        transmit = data.to_bytes(self.__BYTES_DATA, 'little')
        transmit += head.to_bytes(self.__BYTES_HEAD, 'little')
        return transmit

    def is_open(self) -> bool:
        """Return True if the device is open, False otherwise"""
        return self.__device.is_open

    def read(self, no_bytes: int) -> bytes:
        """Read content from device"""
        return self.__device.read(no_bytes)

    def write_wofb(self, data: bytes) -> None:
        """Write content to device without feedback"""
        self.__device.write(data)

    def write_wfb(self, data: bytes, size:int=0) -> bytes:
        """Write all information to device (specific bytes)"""
        num = self.__device.write(data)
        return self.__device.read(num if size <= 0 else size)

    def write_wfb_lf(self, data: bytes) -> bytes:
        """Write all information to device (unlimited bytes until LF)"""
        self.__device.write(data)
        return self.__device.read_until()

    @staticmethod
    def serialize_string(data: str, do_padding: bool) -> list:
        """Serialize a string to bytes"""
        if do_padding:
            data += " "
        chunks = [int.from_bytes(data[i:i + 2].encode('utf-8'), 'big') for i in range(0, len(data), 2)]
        return chunks

    @staticmethod
    def deserialize_string(data: bytes, do_padding: bool) -> str:
        val = data if not do_padding else data[:-1]
        return val.decode('utf8')

    def open(self) -> None:
        """Starting a connection to device"""
        if self.__device.is_open:
            self.__device.close()
        self.__device.open()

    def close(self) -> None:
        """Closing a connection to device"""
        self.__device.close()
