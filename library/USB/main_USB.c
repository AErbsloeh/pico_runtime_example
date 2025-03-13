#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "wrapper/usb_handler.h"


#define MCU_CLK         125000000
#define TEST_CLK        1000
#define USB_FIFO_SIZE   3


int main(){
    // --- Init of Serial COM-Port
    stdio_init_all();
    while(!stdio_usb_connected()){
        sleep_ms(10);
    }  

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
