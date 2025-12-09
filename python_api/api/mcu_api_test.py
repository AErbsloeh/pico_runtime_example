import pytest
from time import sleep
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


def test_check_echo(dut: DeviceAPI):
    test_pattern = "TESTS"
    ret = dut.echo(test_pattern)
    assert ret == test_pattern
    assert len(ret) == len(test_pattern)


def test_check_system_state(dut: DeviceAPI):
    rslt = dut.get_system_state()
    assert rslt == "IDLE"

def test_check_runtime(dut: DeviceAPI):
    wait_time_sec = 0.25

    time0 = dut.get_runtime_sec()
    sleep(wait_time_sec)
    time1 = dut.get_runtime_sec()
    assert time0 < time1
    assert 0.8 * wait_time_sec < time1 - time0 < 1.25 * wait_time_sec


def test_toggle_led(dut: DeviceAPI):
    dut.disable_led()
    assert dut.get_pin_state() == 'NONE'
    dut.toogle_led()
    assert 'LED' in dut.get_pin_state()


def test_enable_led(dut: DeviceAPI):
    dut.disable_led()
    assert dut.get_pin_state() == 'NONE'
    dut.enable_led()
    assert 'LED' in dut.get_pin_state()


def test_diable_led(dut: DeviceAPI):
    dut.enable_led()
    assert 'LED' in dut.get_pin_state()
    dut.disable_led()
    assert dut.get_pin_state() == 'NONE'


if __name__ == "__main__":
    pytest.main([__file__])
