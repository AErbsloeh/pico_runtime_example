#include "hardware_io.h"


int main(){   
    // Init Phase 
	init_gpio_pico();
    init_system();
	run_testbench(TB_MODE_NONE);

    //Main Loop for communication
    while (true){
        process_usb_data(&usb_buffer, LED_TEST);
    }
}
