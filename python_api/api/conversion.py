from dataclasses import dataclass


def _convert_pin_state(state: int) -> str:
    if state == 0:
        return 'NONE'
    else:
        return 'LED_USER'


def _convert_system_state(state: int) -> str:
    state_name = ["ERROR", "RESET", "INIT", "IDLE", "TEST", "DAQ"]
    return state_name[state]


@dataclass
class SystemState:
    """Dataclass for handling the system state of the device
    Attributes:
        pins:       String with enabled GPIO pins (selection from MCU)
        system:     String with actual system state
        runtime:    Float with actual execution runtime after last reset
    """
    pins: str
    system: str
    runtime: float
