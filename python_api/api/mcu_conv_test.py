import pytest
from .mcu_conv import (
    _convert_pin_state,
    _convert_system_state,
    _convert_rp2_adc_value,
    _convert_rp2_temp_value
)


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


def test_adc_rp2_value():
    data_in = [-1, 100, 2048, 4000, 4100]
    check = [0.0, 0.08058608058608059, 1.6504029304029304, 3.2234432234432235, 3.3]
    rslt = [_convert_rp2_adc_value(val) for val in data_in]
    assert rslt == check


def test_adc_rp2_temp():
    data_in = [-1, 100, 2048, 4000, 4100]
    check = [437.22661243463097, 390.4014639244156, -521.752429054579, -1435.7793279739822, -1480.2632190586867]
    rslt = [_convert_rp2_temp_value(val) for val in data_in]
    assert rslt == check


if __name__ == "__main__":
    pytest.main([__file__])
