#include "src/init_system.h"



bool init_gpio_pico(void){
    //Init GPIO + IRQ (Low Level)
    gpio_init(BUTTON_BOARD);
    gpio_set_dir(BUTTON_BOARD, GPIO_IN);
    gpio_pull_up(BUTTON_BOARD);
    gpio_set_slew_rate(BUTTON_BOARD, GPIO_SLEW_RATE_SLOW);
    gpio_set_irq_enabled_with_callback(BUTTON_BOARD, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);


    // --- Init of Serial COM-Port
    stdio_init_all();
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    };

    return true;
}


bool init_system(void){

    return true;
}
