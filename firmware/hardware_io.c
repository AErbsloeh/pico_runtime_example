#include "hardware_io.h"


// ==================== DEVICE DEFINITION =====================

// --- USB PROTOCOL
static char data[USB_FIFO_SIZE] = {0};
static usb_fifo_buffer usb_buffer = {
	.ready = false,
	.length = USB_FIFO_SIZE,
	.position = USB_FIFO_SIZE-1,
	.data = data
};  