#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

// --- Definition of used PINS
#define CLK_TEST_PIN 0
#define BUTTON_BOARD 11

// LED FOR RPi PICO BOARD (Only)
#define BOARD_LED_PIN 25
// LED ON Adafruit KB2040 (only)
#define NEOPIXEL_PIN 17

// --- Configuration of Pico Properties
#define MCU_CLK         125000000
#define TEST_CLK        1000
#define USB_FIFO_SIZE   3

// --- Definition of system variables 
static volatile bool state_led = false;
static uint8_t led_intensity = 16;

