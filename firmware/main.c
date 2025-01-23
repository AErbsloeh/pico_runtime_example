#include "hardware_io.h"


int main(){   
    // Init Phase 
    init_system(128);

    static char data[USB_FIFO_SIZE] = {0};
    static usb_fifo_buffer usb_buffer = {
        .ready = false,
        .length = USB_FIFO_SIZE,
        .position = USB_FIFO_SIZE-1,
        .data = data
    };  

    //Main Loop for communication
    while (true){
        process_usb_data(&usb_buffer, CLK_TEST_PIN);
    }
}
