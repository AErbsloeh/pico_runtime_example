#ifndef HARDWARE_IO_H
#define HARDWARE_IO_H


#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "wrapper/led_control.h"
#include "wrapper/usb_handler.h"
#include "wrapper/tmr_handler.h"
#include "wrapper/pwr_watch.h"

#include "src/init_system.h"
#include "src/testbench.h"
#include "src/gpio_callbacks.h"


#define BLOCK_USB           true
#define TESTBENCH_ACTIVE    TB_MODE_NONE
// ==================== PIN DEFINITION =====================
#define BUTTON_BOARD        11
#define LED_TEST_DEFAULT    17


// ==================== PICO/SYSTEM DEFINITION =====================
// --- Timer  
bool irq_tmr0(repeating_timer_t *rt);
extern repeating_timer_t tmr0;
extern tmr_repeat_irq_t tmr0_hndl;

// ----- USB Communication
#define USB_FIFO_SIZE   3
extern char data[USB_FIFO_SIZE];
extern usb_fifo_t usb_buffer;


// ==================== I2C DEFINITION =====================
//extern i2c_pico_t i2c_mod;


// ==================== SPI DEFINITION =====================
//extern spi_pico_t spi_mod;


#endif
