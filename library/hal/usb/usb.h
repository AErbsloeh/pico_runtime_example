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
} usb_t;


// ========================= FUNCTIONS FOR PROCESSING ===========================
/*! \brief Function to initialize the USB interface
* \return Boolean for initialiation is successful
*/
bool usb_init(void);


/*! \brief Function to wait until USB is connected
* \return Boolean for USB is connected
*/
bool usb_wait_until_connected(void);


/*! \brief Function for handling the FIFO buffer
* \param fifo_buffer    Struct handler for handling FIFO buffer   
*/
void usb_handling_fifo_buffer(usb_t* fifo_buffer);


/*! \brief Function for converting char data for sending out
* \param buffer     Buffer with characters
* \param num_bytes  Number of bytes to send
* \return           Number of written bytes
*/
size_t usb_send_bytes(char* buffer, size_t num_bytes);


/*! \brief Function to calculate a checksum for sending in USB transmission
* \param data	Char array with bytes for USB transmission
* \param len	Length of the char array
* \return 		uint16_t checksum
*/
uint16_t calc_checksum(char* data, size_t len);


#endif
