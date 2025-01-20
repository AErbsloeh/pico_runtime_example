#include "lib/usb_handler.h"


void handling_usb_fifo_buffer(usb_fifo_buffer* fifo_buffer){
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


void process_usb_data(usb_fifo_buffer* fifo_buffer, uint8_t gpio_led){
    //Getting data
    handling_usb_fifo_buffer(fifo_buffer);
    // Datahandler            
    if(fifo_buffer->ready){
        char* buffer = *fifo_buffer->data;
        char val_chck = buffer[2];
        
        if(val_chck == '1'){
            // Turn On LED
            state_led = true;
            printf("LED=on\n");
        }
        else if(val_chck == '0'){
            // Turn Off LED
            state_led = false;
            printf("LED=off\n");
        }
        else{
            printf("Invalid Input!\n");
        }
        gpio_put(gpio_led, state_led);
    }
};


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
