#include <stdio.h>
#include "pico/stdlib.h"
#include "wrapper/usb_handler.h"


// ============================== FUNCTIONS FOR PROCESSING ==============================
void handling_usb_fifo_buffer(usb_fifo_t* fifo_buffer){
    char* buffer = *fifo_buffer->data;
    buffer[fifo_buffer->position] = getchar();

    // Control lines
    if(fifo_buffer->position == 0) {
        fifo_buffer->position = fifo_buffer->length -1;
        fifo_buffer->ready = true;
    } else {
        fifo_buffer->position--;
        fifo_buffer->ready = false;
    }
};


void process_usb_data(usb_fifo_t* fifo_buffer){
    //Getting data
    handling_usb_fifo_buffer(fifo_buffer);
    // Datahandler            
    if(fifo_buffer->ready){
        char* buffer = *fifo_buffer->data;
        char val_chck = buffer[2];
        
        switch(val_chck){
            case '0':
                // Turn Off LED
                set_default_led(false);
                printf("LED off\n");
            break;
            case '1':
                // Turn On LED
                set_default_led(true);
                printf("LED on\n");
            break;
            default:
                // Echo Mode
                for(uint8_t idx = 0; idx < fifo_buffer->length; idx++){
                    putchar(buffer[fifo_buffer->length-1-idx]);
                }
            break;
        }        
    }
};


// ============================== FUNCTIONS FOR COMPRESSED DATA TRANSMISSION ==============================
uint16_t send_uint16_data_to_hexstring(char separator, uint16_t data){
    printf("%c%04x", separator, data & 0xFFFF);
}


uint16_t send_uint16_data_to_hex(char separator, uint16_t data){
    char val_hgh = ((data >> 8) & 0x0000FF) + 0x00;
    char val_low = ((data >> 0) & 0x0000FF) + 0x00;  
    printf("%c%c%c", separator, val_hgh, val_low);
}


int16_t send_int16_data_to_hexstring(char separator, int16_t data){
    printf("%c%04x", separator, data & 0xFFFF);
}


int16_t send_int16_data_to_hex(char separator, int16_t data){
    char val_hgh = ((data >> 8) & 0x0000FF) + 0x00;
    char val_low = ((data >> 0) & 0x0000FF) + 0x00;  
    printf("%c%c%c", separator, val_hgh, val_low);
}
