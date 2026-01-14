def _convert_pin_state(state: int) -> str:
    """Function for converting the pin state
    :param state:   Integer with pin state from MCU
    :return:        String with pin state
    """
    led_name = ['LED_USER']
    if state == 0:
        return 'NONE'
    else:
        ret_text = ''
        for idx, led in enumerate(led_name):
            if state and (1 << idx):
                ret_text += f'{led}' if not len(ret_text) else f'+{led}'
        return ret_text


def _convert_system_state(state: int) -> str:
    """Function for converting the pin state
    :param state:   Integer with pin state from MCU
    :return:        String with pin state
    """
    state_name = ["ERROR", "RESET", "INIT", "IDLE", "TEST", "DAQ"]
    if not 0 <= state < len(state_name):
        raise ValueError(f'Invalid pin state: {state}')
    return state_name[state]
    
   
def _convert_rp2_adc_value(raw: int) -> float:
    """Function for converting the RP2 ADC value from integer to float"""
    if raw >= 65535:
        val0 = 65535
    elif raw < 0:
        val0 = 0
    else:
        val0 = raw
    return val0 * 3.3 / 4095
