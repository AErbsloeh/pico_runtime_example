#ifndef HARDWARE_IO_H
#define HARDWARE_IO_H


#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "wrapper/usb_handler.h"
#include "wrapper/timer_daq.h"

#include "src/init_system.h"
#include "src/testbench.h"


// --- Definition of used PINS
#define BUTTON_BOARD        11
#define LED_TEST_DEFAULT    17


// --- Definition of system variables 
//extern repeating_timer_t tmr0;
//extern tmr_isr_handler_t tmr0_hndl;


// ----- INIT OF USB DEVICES
#define USB_FIFO_SIZE   3
extern char data[USB_FIFO_SIZE];
extern usb_fifo_buffer usb_buffer;


// ============================== FUNCTIONS =========================================
void init_default_led(void);

bool set_default_led(bool state);

bool get_default_led(void);


#endif
