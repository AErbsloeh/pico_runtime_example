def _convert_pin_state(state: int) -> str:
    """Function for converting the pin state
    :param state:   Integer with pin state from MCU
    :return:        String with pin state
    """
    if state == 0:
        return 'NONE'
    else:
        # TODO: Implement logic for several pins
        return 'LED_USER'


def _convert_system_state(state: int) -> str:
    """Function for converting the pin state
    :param state:   Integer with pin state from MCU
    :return:        String with pin state
    """
    state_name = ["ERROR", "RESET", "INIT", "IDLE", "TEST", "DAQ"]
    if not 0 <= state < len(state_name):
        raise ValueError(f'Invalid pin state: {state}')
    return state_name[state]
