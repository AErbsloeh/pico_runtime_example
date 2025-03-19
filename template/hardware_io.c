#include "hardware_io.h"


// ==================== DEVICE DEFINITION =====================
bool state_led = false;


// --- USB PROTOCOL
char data_usb[USB_FIFO_SIZE] = {0};
usb_fifo_buffer usb_buffer = {
	.ready = false,
	.length = USB_FIFO_SIZE,
	.position = USB_FIFO_SIZE-1,
	.data = data_usb
};  
