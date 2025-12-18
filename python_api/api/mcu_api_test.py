import pytest
from time import sleep
from api.mcu_api import DeviceAPI


@pytest.fixture(scope="session", autouse=True)
def before_all_tests():
    DeviceAPI().do_reset()


@pytest.fixture
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
    rslt = dut._get_system_state()
    assert rslt == "IDLE"


def test_check_system_clock(dut: DeviceAPI):
    rslt = dut._get_system_clock_khz()
    assert rslt in [125000, 150000]


def test_check_runtime(dut: DeviceAPI):
    wait_time_sec = 0.25

    time0 = dut._get_runtime_sec()
    sleep(wait_time_sec)
    time1 = dut._get_runtime_sec()
    assert time0 < time1
    assert 0.8 * wait_time_sec < time1 - time0 < 1.25 * wait_time_sec


def test_check_system_state_class(dut: DeviceAPI):
    rslt = dut.get_state()
    assert rslt.system == "IDLE"
    assert rslt.pins == "LED_USER"
    assert rslt.runtime > 0
    assert rslt.clock in [125000, 150000]


def test_toggle_led(dut: DeviceAPI):
    dut.disable_led()
    assert dut._get_pin_state() == 'NONE'
    dut.toggle_led()
    assert 'LED' in dut._get_pin_state()


def test_enable_led(dut: DeviceAPI):
    dut.disable_led()
    assert dut._get_pin_state() == 'NONE'
    dut.enable_led()
    assert 'LED' in dut._get_pin_state()


def test_diable_led(dut: DeviceAPI):
    dut.enable_led()
    assert 'LED' in dut._get_pin_state()
    dut.disable_led()
    assert dut._get_pin_state() == 'NONE'


def test_control_daq(dut: DeviceAPI):
    dut.update_daq_sampling_rate(2.)
    dut.start_daq()
    assert dut._get_system_state() == 'DAQ'
    sleep(0.5)
    while dut._get_pin_state() == 'LED_USER':
        sleep(0.05)
    sleep(0.5)
    while dut._get_pin_state() == 'NONE':
        sleep(0.05)
    sleep(0.5)
    dut.stop_daq()
    sleep(0.5)


if __name__ == "__main__":
    pytest.main([__file__])
