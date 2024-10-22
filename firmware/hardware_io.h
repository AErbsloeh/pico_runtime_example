#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#define CLK_TEST_PIN 0
#define BUTTON_BOARD 11
#define NEOPIXEL_PIN 17

#define MCU_CLK 1000000
#define TIMER_CLK 1
#define USB_FIFO_SIZE 3

static volatile bool state_led = false;
static uint8_t led_intensity = 16;

