#include "lib/power_watch.h"


void gpio_isr_pwr_monitoring(uint gpio, uint32_t events){
    /* ADD CODE HERE WITH RIGHT STRUCT HANDLER */
    
    /*Example of FUNC
    system_power_stable = false;
    
    while(true){
        sleep_ms(250);
        gpio_put(LED_ERR_PIN, false);
        sleep_ms(250);
        gpio_put(LED_ERR_PIN, true);        
        printf("Please restart the System!\n");
    };*/
}


bool monitor_system_power_pgd_start(uint8_t pin_pgd){
    bool power_not_ready = true;

    for(uint16_t idx=0; idx < 10000; idx++){
        sleep_us(10);
        power_not_ready = gpio_get(pin_pgd);
    }

    if(!power_not_ready){
        sleep_ms(1000);
        gpio_set_irq_enabled(pin_pgd, GPIO_IRQ_EDGE_RISE, true);
        //gpio_set_irq_enabled_with_callback(pin_pgd, GPIO_IRQ_EDGE_RISE, true, &gpio_isr_pwr_monitoring);
    }
    return !power_not_ready;
}


bool init_system_power_single(power_single_t *config){
    gpio_init(config->pin_en);
    gpio_set_dir(config->pin_en, true);    
    gpio_put(config->pin_en, false);

    if(config->use_pgd){
        gpio_init(config->pin_pgd);
        gpio_set_dir(config->pin_pgd, false);
        gpio_pull_up(config->pin_pgd);
    }

    config->init_done = true;
    return config->init_done;
}


bool enable_system_power_single(power_single_t *config){
    if(!config->init_done){
        init_system_power_single(config);
    }

    sleep_ms(500);
    gpio_put(config->pin_en, true);

    bool state = true;
    if(config->use_pgd){
        state = monitor_system_power_pgd_start(config->pin_pgd);
    }
    if(!state) {
        bool state_led = false;
        disable_system_power_single(config);
        while(!state){
            gpio_put(config->pin_led, state_led);
            state_led = !state_led;
            sleep_ms(1000);
            printf("... system board not available. Please check!");
        }
    }
    config->state = state;
    return config->state;
}


bool disable_system_power_single(power_single_t *config){
    gpio_put(config->pin_en, false);
    sleep_us(10);

    config->state = false;
    return config->state;
}


bool init_system_power_dual(power_dual_t *config){
    gpio_init(config->pin_en_reg);
    gpio_set_dir(config->pin_en_reg, true);    
    gpio_put(config->pin_en_reg, false);

    gpio_init(config->pin_en_ldo);
    gpio_set_dir(config->pin_en_ldo, true);    
    gpio_put(config->pin_en_ldo, false);

    if(config->use_pgd){
        gpio_init(config->pin_pgd);
        gpio_set_dir(config->pin_pgd, false);
        gpio_pull_up(config->pin_pgd);
    }

    config->init_done = true;
    return config->init_done;
}


bool enable_system_power_dual(power_dual_t *config){
    if(!config->init_done){
        init_system_power_dual(config);
    }

    sleep_ms(500);
    gpio_put(config->pin_en_reg, true);
    sleep_ms(500);
    gpio_put(config->pin_en_ldo, true);

    bool state = true;
    if(config->use_pgd){
        bool state_led = false;
        state = monitor_system_power_pgd_start(config->pin_pgd);
        while(!state){
            gpio_put(config->pin_led, state_led);
            state_led = !state_led;
            sleep_ms(1000);
            printf("... system board not available. Please check!");
        }
    }
    if(!state) {
        disable_system_power_dual(config);
    }
    config->state = state;
    return config->state;
}


bool disable_system_power_dual(power_dual_t *config){
    gpio_put(config->pin_en_ldo, false);
    sleep_ms(10);
    gpio_put(config->pin_en_reg, false);
    sleep_ms(10);

    config->state = false;
    return config->state;
}
