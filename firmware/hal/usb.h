#ifndef HAL_USB_H_
#define HAL_USB_H_


#include "pico/stdlib.h"


// ============================== DEFINITIONS ==============================
/*! \brief Struct handler for configuring USB FIFO Buffer
* \param ready      Boolean if FIFO buffer has new data is ready for processing
* \param length     Length of the used FIFO buffer
* \param position   Actual position for write new data in ring buffer 
* \param data       FIFO Buffer      
*/
typedef struct{
    bool ready;
    uint8_t length;
    uint8_t position;
    char* data[];
} usb_fifo_t;


// ============================== FUNCTIONS FOR PROCESSING ==============================
/*! \brief Function for handling the FIFO buffer
* \param fifo_buffer    Struct handler for handling FIFO buffer   
*/
void handling_usb_fifo_buffer(usb_fifo_t* fifo_buffer);


// ============================== FUNCTIONS FOR COMPRESSED DATA TRANSMISSION ==============================
/*! \brief Function for converting char data for sending out
* \param buffer     Buffer with characters
* \param num_bytes  Number of bytes to send
* \return           Number of written bytes
*/
size_t send_bytes(char* buffer, size_t num_bytes);


#endif
