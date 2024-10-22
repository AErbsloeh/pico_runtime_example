#include <stdio.h>
#include "pico/stdlib.h"


typedef struct{
    bool ready;
    uint8_t length;
    uint8_t position;
    char* data[];
} usb_fifo_buffer;


void handling_usb_fifo_buffer(usb_fifo_buffer* fifo_buffer);
void process_usb_data(usb_fifo_buffer* fifo_buffer);

uint16_t send_uint16_data_to_hexstring(char separator, uint16_t data);
uint16_t send_uint16_data_to_hex(char separator, uint16_t data);
int16_t send_int16_data_to_hexstring(char separator, int16_t data);
int16_t send_int16_data_to_hex(char separator, int16_t data);
