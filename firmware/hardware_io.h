#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

// --- Definition of used PINS
#define CLK_TEST_PIN 0
#define BUTTON_BOARD 11
#define NEOPIXEL_PIN 17

// --- Configuration of Pico Properties
#define MCU_CLK 1000000
#define TIMER_CLK 1
#define USB_FIFO_SIZE 3

// --- Definition of system variables 
static volatile bool state_led = false;
static uint8_t led_intensity = 16;

