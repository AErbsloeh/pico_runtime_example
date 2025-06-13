#include "src/init_system.h"
#ifdef PICO_CYW43_SUPPORTED
    #include "pico/cyw43_arch.h"
#endif
#include "hardware_io.h"


bool init_gpio_pico(){
    // --- Init of Wireless Module (if used)
    #ifdef PICO_CYW43_SUPPORTED 
        if (cyw43_arch_init()) {
            return false;
        }
    #endif 
    
    // --- Init of GPIOs
    init_default_led(LED_TEST_DEFAULT);

    // --- Init GPIO + IRQ (Low Level)
    gpio_init(BUTTON_BOARD);
    gpio_set_dir(BUTTON_BOARD, GPIO_IN);
    gpio_pull_up(BUTTON_BOARD);
    gpio_set_slew_rate(BUTTON_BOARD, GPIO_SLEW_RATE_SLOW);
    gpio_set_irq_enabled_with_callback(BUTTON_BOARD, GPIO_IRQ_EDGE_FALL, true, &irq_gpio_callbacks);


    // --- Init of Serial COM-Port
    stdio_init_all();
	// Wait until USB is connected
    if (BLOCK_USB)
        while(!stdio_usb_connected()){
            sleep_ms(10);
        };

    return true;
}


bool init_system(void){
    uint8_t num_init_done = 0;

    // --- Init of Timer
    if(enable_timer_irq(&tmr0_hndl)){
        num_init_done++;
    };

    // --- Blocking Routine if init is not completed
    if(num_init_done == 1){
        sleep_ms(10);
        set_default_led(true);
        return true;

    } else {
        while(true){
            printf("... Init System not done yet\n");
            sleep_ms(1000);
            set_default_led(!get_default_led());
        }
        return false;
    }
    
}
