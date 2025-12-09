#include "src/init_system.h"
#ifdef PICO_CYW43_SUPPORTED
    #include "pico/cyw43_arch.h"
#endif
#include "hardware_io.h"
#include "hardware/watchdog.h"


void reset_pico_mcu(bool wait_until_done){
    set_system_state(STATE_RESET);
    watchdog_enable(100, 1);
    if(wait_until_done){
        while(true){
            tight_loop_contents();
        }
    };
}


bool init_gpio_pico(bool block_usb){
    set_system_state(STATE_NONE);
    // --- Init of Wireless Module (if used)
    #ifdef PICO_CYW43_SUPPORTED 
        if (cyw43_arch_init()) {
            return false;
        }
    #endif
    
    // --- Init of GPIOs
    init_default_led(LED_TEST_DEFAULT);

    // --- Init GPIO + IRQ (Low Level)
    /*gpio_init(BUTTON_BOARD);
    gpio_set_dir(BUTTON_BOARD, GPIO_IN);
    gpio_pull_up(BUTTON_BOARD);
    gpio_set_slew_rate(BUTTON_BOARD, GPIO_SLEW_RATE_SLOW);
    gpio_set_irq_enabled_with_callback(BUTTON_BOARD, GPIO_IRQ_EDGE_FALL, true, &irq_gpio_callbacks);*/

    // --- Init of Serial COM-Port
    stdio_init_all();
    stdio_set_translate_crlf(&stdio_usb, false);
    if (block_usb){
        // Wait until USB is connected
        while(!stdio_usb_connected()){
            sleep_ms(1);
        };
    };
    return true;
}


bool init_system(void){
    set_system_state(STATE_IDLE);
    uint8_t num_init_done = 0;

    // --- Init of Timer
    if(init_timer_irq(&tmr0_hndl)){
        num_init_done++;
    };

    // --- Blocking Routine if init is not completed
    if(num_init_done == 1){
        sleep_ms(10);
        set_system_state(STATE_IDLE);
        return true;
    } else {
        while(true){
            printf("... Init System not done yet\n");
            set_system_state(STATE_ERROR);
            sleep_ms(1000);
            set_default_led(!get_default_led());
        }
        return false;
    }
}


uint64_t get_runtime_ms(void){
    absolute_time_t now = get_absolute_time();
    return to_us_since_boot(now);
}


system_state_t get_system_state(void){
    return system_state;
}


bool set_system_state(system_state_t new_state){
    bool valid_state = false;
    
    if(system_state != new_state){
        system_state = new_state;
        switch(new_state){
            case STATE_NONE:
                set_default_led(false);
                valid_state = true;
                break;
            case STATE_INIT:
                set_default_led(false);
                valid_state = true;
                break;
            case STATE_IDLE:
                set_default_led(true);
                valid_state = true;
                break;
            case STATE_ERROR:
                set_default_led(true);
                valid_state = false;
                break;
            default:
                set_default_led(false);
                valid_state = false;
                break;
        }
        return valid_state;
    } else {
        return false;
    };    
}
