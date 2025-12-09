#include <stdio.h>
#include "hal/usb.h"


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


// ============================== FUNCTIONS FOR COMPRESSED DATA TRANSMISSION ==============================
size_t send_bytes(char* buffer, size_t num_bytes){
    size_t written = fwrite(buffer, 1, num_bytes, stdout);
    fflush(stdout);
    return written;
}
