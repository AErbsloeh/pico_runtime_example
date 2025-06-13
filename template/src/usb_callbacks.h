#ifndef USB_CALLBACKS_H_
#define USB_CALLBACKS_H_


#include "pico/stdlib.h"
#include "hal/usb.h"


/*! \brief Function for processing the output of FIFO buffer
* \param fifo_buffer    Struct handler for handling FIFO buffer   
*/
bool apply_usb_callback(usb_fifo_t* fifo_buffer);


#endif
