import sys
from serial import Serial, SerialException, STOPBITS_ONE, EIGHTBITS, PARITY_NONE
from glob import glob


def scan_available_serial_ports() -> list:
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = Serial(port)
            s.close()
            result.append(port)
        except (OSError, SerialException):
            pass
    return result


class HandlerUSB:
    __BYTES_HEAD: int = 1
    __BYTES_DATA: int = 2
    __device: Serial

    def __init__(self, com_name: str, baud: int=115200) -> None:
        """Init. of the device with name and baudrate of the device
        :param com_name:    String with the serial port name of the used device
        :param baud:        Baud rate of the communiation between API and device [default: 115.200]
        """
        self.__device = Serial(
            port=com_name,
            baudrate=baud,
            parity=PARITY_NONE,
            stopbits=STOPBITS_ONE,
            bytesize=EIGHTBITS,
            inter_byte_timeout=0.5
        )

    @property
    def total_num_bytes(self) -> int:
        """Returning the total number of bytes for each transmission"""
        return self.__BYTES_DATA + self.__BYTES_HEAD

    @property
    def is_com_port_active(self):
        """Boolean for checking if serial communication is open and used"""
        return self.__device.is_open

    def _read(self, no_bytes: int):
        """Read content from device"""
        return self.__device._read(no_bytes)

    def _write_wofb(self, data: bytes) -> None:
        """Write content to device without feedback"""
        self.__device.write(data)

    def _write_wfb(self, data: bytes):
        """Write all information to device (specific bytes)"""
        self.__device.write(data)
        dev_out = self.__device._read(len(data))
        return dev_out

    def _write_wfb_lf(self, data: bytes):
        """Write all information to device (unlimited bytes until LF)"""
        self.__device.write(data)
        dev_out = self.__device.read_until()
        return dev_out

    def _write_wfb_hex(self, head: int, data: int) -> bytes:
        """Write content to FPGA/MCU for specific custom-made task"""
        transmit_byte = head.to_bytes(self.__BYTES_HEAD, 'little')
        transmit_byte += data.to_bytes(self.__BYTES_DATA, 'big')
        self.__device.write(transmit_byte)
        out = self.__device._read(len(transmit_byte))
        return out

    def open(self):
        """Starting a connection to device"""
        if self.__device.is_open:
            self.__device.close()
        self.__device.open()

    def close(self):
        """Closing a connection to device"""
        self.__device.close()

    def echo(self, data: str) -> str:
        """"""
        ret = self._write_wfb(bytes([0x01, 0x00, 0x00]))
        return str(ret)

    def get_system_state(self) -> str:
        ret = self._write_wfb(bytes([0x01, 0x00, 0x00]))
        return str(ret)

    def enable_led(self):
        """"""
        self.__device.write(bytes([0x03, 0x00, 0x00]))

    def disable_led(self):
        """"""
        self.__device.write(bytes([0x04, 0x00, 0x00]))

    def toogle_led(self):
        """"""
        self.__device.write(bytes([0x05, 0x00, 0x00]))
