#include "hardware_io.h"


int main(){   
    // Init Phase 
    init_gpio_pico(true);
    init_system();
    sleep_ms(100);
    run_testbench(TB_MODE_NONE);

    // Main Loop
    while (true) {
        sleep_ms(500);   
        //process_usb_data(&usb_buffer, LED_TEST);
    }
}
