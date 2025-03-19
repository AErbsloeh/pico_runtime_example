#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "wrapper/usb_handler.h"
#include "wrapper/timer_daq.h"

#include "src/init_system.h"
#include "src/testbench.h"

// --- Definition of used PINS
#define BUTTON_BOARD    11
#define LED_TEST        17


// --- Definition of system variables 
extern bool state_led;


// Here: List of construct for using devices/sensors (IMPORTANT: Load as extern device and write the code in *.c)
// Example: extern spi_device_handler_t spi_hndl0;
// ----- INIT OF USB DEVICES
#define USB_FIFO_SIZE   3
extern char data[USB_FIFO_SIZE];
extern usb_fifo_buffer usb_buffer;
