#include "hardware_io.h"


int main(){   
    // Init Phase 
    init_gpio_pico(false);
    init_system();
    run_testbench(TB_NONE);

    // Main Loop
    while (true) {  
        apply_usb_callback(&usb_buffer);
    };
}
