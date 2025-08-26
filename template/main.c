#include "hardware_io.h"


int main(){   
    // Init Phase 
    init_gpio_pico();
    init_system();
    run_testbench(TESTBENCH_ACTIVE);

    // Main Loop
    while (true) {  
        apply_usb_callback(&usb_buffer);
    }
}
