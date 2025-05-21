#ifndef USB_HANDLER_H
#define USB_HANDLER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "wrapper/led_control.h"


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
} usb_fifo_buffer;


/*! \brief Function for handling the FIFO buffer
* \param fifo_buffer    Struct handler for handling FIFO buffer   
*/
void handling_usb_fifo_buffer(usb_fifo_buffer* fifo_buffer);


/*! \brief Function for processing the output of FIFO buffer
* \param fifo_buffer    Struct handler for handling FIFO buffer   
*/
void process_usb_data(usb_fifo_buffer* fifo_buffer);


/*! \brief Function for converting uint16 data with used separator into hex string and sending out
* \param separator      One char value for separating on host computer
* \param data           uint16 data value   
*/
uint16_t send_uint16_data_to_hexstring(char separator, uint16_t data);


/*! \brief Function for converting uint16 data with used separator into hex and sending out
* \param separator      One char value for separating on host computer
* \param data           uint16 data value   
*/
uint16_t send_uint16_data_to_hex(char separator, uint16_t data);


/*! \brief Function for converting int16 data with used separator into hex string and sending out
* \param separator      One char value for separating on host computer
* \param data           uint16 data value   
*/
int16_t send_int16_data_to_hexstring(char separator, int16_t data);


/*! \brief Function for converting int16 data with used separator into hex and sending out
* \param separator      One char value for separating on host computer
* \param data           uint16 data value   
*/
int16_t send_int16_data_to_hex(char separator, int16_t data);

#endif
