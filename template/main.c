#include "hardware_io.h"


int main(){   
    // Init Phase 
    init_gpio_pico(false);
    init_system();
    run_testbench(TESTBENCH_ACTIVE);

    // Main Loop
    while (true) {  
        process_usb_data(&usb_buffer);
    }
}
