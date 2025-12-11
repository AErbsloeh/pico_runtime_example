import pytest
from .mcu_conv import _convert_pin_state, _convert_system_state


def test_pin_states():
    data_in = [idx for idx in range(2)]
    check = ["NONE", "LED_USER"]
    rslt = [_convert_pin_state(idx) for idx in data_in]
    assert rslt == check


def test_system_states():
    data_in = [idx for idx in range(6)]
    check = ["ERROR", "RESET", "INIT", "IDLE", "TEST", "DAQ"]
    rslt = [_convert_system_state(idx) for idx in data_in]
    assert rslt == check


if __name__ == "__main__":
    pytest.main([__file__])
