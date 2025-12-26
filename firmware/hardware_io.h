#ifndef HARDWARE_IO_H_
#define HARDWARE_IO_H_


#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hal/led/led.h"
#include "hal/tmr/tmr.h"
#include "hal/usb/usb.h"

#include "src/init_system.h"
#include "src/testbench.h"
#include "src/daq_sample.h"

#define FIRMWARE_VERSION_MAJOR 0
#define FIRMWARE_VERSION_MINOR 1
extern system_state_t system_state;
// ==================== PIN DEFINITION =====================
#define BUTTON_BOARD        11
#define LED_TEST_DEFAULT    25

// ==================== I2C DEFINITION =====================
//extern i2c_rp2_t i2c_mod;


// ==================== SPI DEFINITION =====================
//extern spi_rp2_t spi_mod;


// ================ PICO/SYSTEM DEFINITION =================
// --- USB Communication
#define USB_FIFO_SIZE 3
extern char data[USB_FIFO_SIZE];
extern usb_rp2_t usb_buffer;

// --- DAQ Sampling
extern daq_data_t daq_sample_data;
extern tmr_repeat_irq_t tmr_daq0_hndl;

#endif
