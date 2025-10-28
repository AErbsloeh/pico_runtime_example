#include "hardware_io.h"


system_state_t system_state = STATE_NONE;
// ==================== PICO/SYSTEM DEFINITION =====================
// --- Timer
bool irq_tmr0(repeating_timer_t *rt){
    printf("Timer IRQ\n");  
    return true;    
};
repeating_timer_t tmr0;
tmr_repeat_irq_t tmr0_hndl = {
    .timer = &tmr0,
    .irq_number = 0,
    .period_us = 250000,
    .alarm_done = false,
    .enable_state = true,
    .init_done = false,
    .func_irq = irq_tmr0
};


// --- USB PROTOCOL
char data_usb[USB_FIFO_SIZE] = {0};
usb_fifo_t usb_buffer = {
	.ready = false,
	.length = USB_FIFO_SIZE,
	.position = USB_FIFO_SIZE-1,
	.data = data_usb
};  


// ==================== I2C DEFINITION =====================



// ==================== SPI DEFINITION =====================

