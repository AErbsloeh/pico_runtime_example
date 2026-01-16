#include "hardware_io.h"


system_state_t system_state = STATE_ERROR;
// ==================== I2C DEFINITION =====================
//spi_rp2_t spi_mod = {};


// ==================== SPI DEFINITION =====================
//i2c_rp2_t i2c_mod = {};


// =============== PICO/SYSTEM DEFINITION ==================
// --- Internal Temp Sensor
rp2_adc_t adc_temp = {
    .adc_channel = RP2_ADC_TEMP,
    .init_done = false
};

// --- USB PROTOCOL
char data_usb[USB_FIFO_SIZE] = {0};
usb_rp2_t usb_buffer = {
	.ready = false,
	.length = USB_FIFO_SIZE,
	.position = USB_FIFO_SIZE-1,
	.data = data_usb
};  

// --- DAQ Sampling
daq_data_t daq_sample_data = {
    .packet_id = 0xA0,
    .iteration = 0,
    .runtime = 0,
    .value_ch0 = 0,
    .value_ch1 = 0
};
bool irq_tmr_daq0(repeating_timer_t *rt){
    daq_sample_data.iteration ++;
    daq_sample_data.runtime = get_runtime_ms();
    daq_sample_data.value_ch0 += 4;
    daq_sample_data.value_ch1 -= 4;
    send_daq_data_usb(&daq_sample_data);

    toggle_state_default_led();
    return true;    
};
repeating_timer_t tmr_daq0;
tmr_repeat_irq_t tmr_daq0_hndl = {
    .timer = &tmr_daq0,
    .irq_number = 0,
    .period_us = -250000,
    .alarm_done = false,
    .enable_state = false,
    .init_done = false,
    .func_irq = irq_tmr_daq0
};
