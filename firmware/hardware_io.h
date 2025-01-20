#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"


// --- Definition of used PINS
#define BUTTON_BOARD 11


// --- Configuration of Pico Properties
#define MCU_CLK         125000000
#define USB_FIFO_SIZE   3


// --- Definition of system variables 
static volatile bool state_led = false;
static uint8_t led_intensity = 16;

