import pytest
from mcu_api import scan_available_serial_ports, HandlerUSB


@pytest.fixture()
def dut():
    com_list = scan_available_serial_ports()
    mcu_api = HandlerUSB(
        com_name=com_list[0]
    )
    mcu_api.open()
    yield mcu_api


def test_num_bytes(dut: HandlerUSB):
    assert dut.total_num_bytes == 3


def test_check_opened(dut: HandlerUSB):
    assert dut.is_com_port_active == True


def test_toggle_led(dut: HandlerUSB):
    dut.toogle_led()


if __name__ == "__main__":
    pytest.main([__file__])
