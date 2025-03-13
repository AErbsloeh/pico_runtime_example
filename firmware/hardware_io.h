#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/clocks.h"

#include "lib/ws2812.h"
#include "lib/blink.pio.h"
#include "lib/usb_handler.h"
#include "lib/timer_daq.h"

#include "src/init_system.h"
#include "src/testbench.h"

// --- Definition of used PINS
#define BUTTON_BOARD 11


// --- Definition of system variables 
static volatile bool state_led = false;
static uint8_t led_intensity = 16;


// Here: List of construct for using devices/sensors (IMPORTANT: Load as extern device and write the code in *.c)
// Example: extern spi_device_handler_t spi_hndl0;
