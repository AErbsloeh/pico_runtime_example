import pytest
from mcu_api import DeviceAPI


@pytest.fixture(scope="module")
def init_before_all_tests():
    DeviceAPI().do_reset()


@pytest.fixture()
def dut():
    mcu_api = DeviceAPI(
        com_name="AUTOCOM"
    )
    yield mcu_api


def test_num_bytes(dut: DeviceAPI):
    assert dut.total_num_bytes == 3


def test_check_opened(dut: DeviceAPI):
    assert dut.is_com_port_active == True


def test_toggle_led(dut: DeviceAPI):
    dut.toogle_led()


if __name__ == "__main__":
    pytest.main([__file__])
