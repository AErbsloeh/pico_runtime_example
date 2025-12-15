#include "hal/usb/usb.h"
#include <stdio.h>


// ============================== FUNCTIONS FOR PROCESSING ==============================
bool usb_init(void){
    stdio_init_all();
	stdio_set_translate_crlf(&stdio_usb, false);
    return true;
}


bool usb_wait_until_connected(void){
    while (!stdio_usb_connected()){
        sleep_ms(1);
    };
    return true;
}


void usb_handling_fifo_buffer(usb_t* fifo_buffer){
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


size_t usb_send_bytes(char* buffer, size_t num_bytes){
    size_t written = fwrite(buffer, 1, num_bytes, stdout);
    fflush(stdout);
    return written;
}


uint16_t calc_checksum(char* data, size_t len){
    uint16_t sum = 0;    
    for (size_t idx=0; idx < len; idx++){
        sum += data[idx];
    };
    return sum;
}
