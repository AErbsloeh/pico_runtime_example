#include "hardware_io.h"
#include "sens/btstack_spp_counter.h"


int main(){   
    // Init Phase 
    init_gpio_pico();
    init_system();
    run_testbench(TESTBENCH_ACTIVE);

    if(btstack_spp_init()){
        printf("SPP init\n");
    }
    btstack_spp_start();

    // Main Loop
    while (true) {  
        apply_usb_callback(&usb_buffer);
    }
}
