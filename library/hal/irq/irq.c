#include "wrapper/irq_handler.h"

main_irq_handler_t main_irq_handler = {
    .init_done = false,
    .gpio_callback_array = {},
    .pwm_wrap_callback_array = {}
};



// ====================== FUNCTIONS ====================== 

void main_irq_callback(uint gpio, uint32_t event_mask){
    //printf("Main IRQ Callback: Function %d\n", main_irq_handler.gpio_callback_array[gpio]);
    if(main_irq_handler.gpio_callback_array[gpio]){
        main_irq_handler.gpio_callback_array[gpio]();
    }
}

void construct_irq_add_gpio(uint8_t pin, uint32_t event_mask, void (*callback)(void)){
    gpio_set_irq_enabled_with_callback(pin, event_mask, true, main_irq_callback);
    main_irq_handler.gpio_callback_array[pin] = callback;
    printf("Added pin %d\n", pin);
}

void construct_irq_add_pwm_wrap(uint8_t pin, void (*callback)(void)){
    printf("Added pin WRAP %d\n", pin);
    main_irq_handler.pwm_wrap_callback_array[pwm_gpio_to_slice_num(pin)] = callback;
}

void pwm_wrap_callback(void){
    //printf("PWM IRQ Wrap Callback\n");
    uint32_t status = pwm_get_irq_status_mask();
    
    for (uint slice = 0; slice < NUM_PWM_SLICES; ++slice) {
        if (status & (1u << slice)) {
            pwm_clear_irq(slice);
            //printf("PWM IRQ Wrap Callback: Slice %d\n", slice);
            if (main_irq_handler.pwm_wrap_callback_array[slice]) {
                main_irq_handler.pwm_wrap_callback_array[slice]();
            }
        }
    }
}

bool main_irq_init(main_irq_handler_t* handler){
    irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), pwm_wrap_callback);
    irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);
    //irq_set_priority(PWM_DEFAULT_IRQ_NUM(), 0);
    handler->init_done = true;
    return handler->init_done;
};


main_irq_handler_t* get_main_irq_handler(void){
    return &main_irq_handler;
}