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
*/
void send_bytes(char* buffer, size_t num_bytes);


/*! \brief Function for converting uint16 data with used separator into hex string and sending out
* \param separator      One char value for separating on host computer
*/
void send_uint16_data_to_hexstring(char separator, uint16_t data);


/*! \brief Function for converting uint16 data with used separator into hex and sending out
* \param separator      One char value for separating on host computer  
*/
void send_uint16_data_to_hex(char separator, uint16_t data);


/*! \brief Function for converting int16 data with used separator into hex string and sending out
* \param separator      One char value for separating on host computer  
*/
void send_int16_data_to_hexstring(char separator, int16_t data);


/*! \brief Function for converting int16 data with used separator into hex and sending out
* \param separator      One char value for separating on host computer 
*/
void send_int16_data_to_hex(char separator, int16_t data);


#endif
