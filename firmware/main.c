#include "hardware_io.h"
#include "callbacks/rpc_callbacks.h"
#ifdef ADD_CYW43_SUPPORT
    #include "pico/cyw43_arch.h"
#endif

int main(){   
    #ifdef ADD_CYW43_SUPPORT
        if (cyw43_arch_init()) {
            return -1;
        }
    #endif

    // Init Phase 
    init_gpio_pico(false);
    init_system();
    run_testbench(TB_NONE);    

    // Main Loop
    while (true) {  
        usb_handling_fifo_buffer(&usb_buffer);
        apply_rpc_callback(*usb_buffer.data, usb_buffer.length, usb_buffer.ready);
    };
}
