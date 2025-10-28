#ifndef HARDWARE_IO_H
#define HARDWARE_IO_H


#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hal/led.h"
#include "hal/tmr.h"

#include "src/pwr_watch.h"
#include "src/init_system.h"
#include "src/testbench.h"
#include "src/gpio_callbacks.h"
#include "src/usb_callbacks.h"


#define BLOCK_USB           true
#define TESTBENCH_ACTIVE    TB_MODE_NONE
extern system_state_t system_state;
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
