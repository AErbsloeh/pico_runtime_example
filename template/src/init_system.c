#include "src/init_system.h"


// --- Definition of Callback Functions of ISR
void isr_gpio_button(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    printf("GPIO IRQ done");
}


void gpio_callback(uint gpio, uint32_t events){
	switch(gpio){
			case BUTTON_BOARD: isr_gpio_button(gpio, events);	break;		
	};	
}


bool init_gpio_pico(bool wait_until_usb_connected){
    // --- Init of Wireless Module (if used)
    if(PICO_BOARD == "pico2_w" || PICO_BOARD == "pico_w") {
        if (cyw43_arch_init()) {
            return -1;
        }
    }   

    // --- Init of GPIOs
    init_default_led(LED_TEST_DEFAULT);

    // --- Init GPIO + IRQ (Low Level)
    gpio_init(BUTTON_BOARD);
    gpio_set_dir(BUTTON_BOARD, GPIO_IN);
    gpio_pull_up(BUTTON_BOARD);
    gpio_set_slew_rate(BUTTON_BOARD, GPIO_SLEW_RATE_SLOW);
    gpio_set_irq_enabled_with_callback(BUTTON_BOARD, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);


    // --- Init of Serial COM-Port
    stdio_init_all();
	// Wait until USB is connected
    if (wait_until_usb_connected)
        while(!stdio_usb_connected()){
            sleep_ms(10);
        };

    return true;
}


bool init_system(void){
    uint8_t num_init_done = 0;

    if(enable_timer_irq(&tmr0_example)){
        printf("Timer ISR done\n");
        num_init_done++;
    };

    return num_init_done == 1;
}
