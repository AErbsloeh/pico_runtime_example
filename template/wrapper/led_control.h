#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_


#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"


// ============================== INTERNAL FUNCTIONS =========================================
/*! Function for initializing the default LED.
 * @param led_pin: The GPIO pin number for the LED (if not cyw43 LED is used).
 * @return None
 */
void init_default_led(uint8_t led_pin);


/*! Function for setting the state of the default LED.
 * @param state: The state to set the LED to (true for on, false for off).
 * @return       The state of the LED after setting it.
 */
bool set_default_led(bool state);


/*! Function for getting the state of the default LED.
 * @param state: 
 * @return          The actual state of the LED.
 */
bool get_default_led(void);


#endif
