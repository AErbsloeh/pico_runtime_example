#include "hal/tmr.h"
#include "hal/led.h"


/* --------------- EXAMPLE FOR USING A TIMER --------------- */
bool tmr_irq_routine_example(repeating_timer_t *rt){
    set_default_led(!get_default_led());  
    return true;    
};


/* --------------- CODE FOR TIMER ALARM --------------- */
bool alarm_done = false;
static void init_timer_irq_alarm(void) {
    hw_clear_bits(&timer_hw->intr, 1u);
    alarm_done = true;
};


/* --------------- CODE FOR INIT TIMER --------------- */
bool enable_timer_irq(tmr_repeat_irq_t* handler){
    if(handler->enable_state && !irq_is_enabled(handler->irq_number)){
        hw_set_bits(&timer_hw->inte, 1u);
        irq_set_exclusive_handler(handler->irq_number, init_timer_irq_alarm);
        irq_set_enabled(handler->irq_number, true);
        timer_hw->alarm[0] = timer_hw->timerawl + 1000000;

        // Register control (wait until done)
        alarm_done = false;
        while(!alarm_done){
            sleep_ms(1);
        }

        // Register        
        if (!add_repeating_timer_us(handler->period_us, handler->func_irq, NULL, handler->timer)) {
            handler->init_done = false;
            handler->alarm_done = false;
        } else {
            handler->init_done = true;
            handler->alarm_done = true;
        }
    } else {
        // --- Timer is already enabled
        sleep_ms(1);
    }    
    return handler->init_done;
};    


bool disable_timer_irq(tmr_repeat_irq_t* handler){
    if(irq_is_enabled(handler->irq_number)){
        // --- Disabling the timer
        irq_set_enabled(handler->irq_number, false);
        alarm_done = true;
        while(alarm_done){
            sleep_ms(1);
        }
        handler->alarm_done = alarm_done;
        irq_clear(handler->irq_number);
        handler->init_done = false;
    } else {
        // --- Timer is disabled
        sleep_ms(1);
    }     
    return handler->init_done;
};
