#include "hal/tmr/tmr.h"


/* --------------- EXAMPLE FOR USING A TIMER --------------- */
bool tmr_irq_routine_example(repeating_timer_t *rt){
    return true;    
};


/* --------------- CODE FOR TIMER ALARM --------------- */
bool alarm_done = false;
static void init_timer_irq_alarm(void) {
    hw_clear_bits(&timer_hw->intr, 1u);
    alarm_done = true;
};


/* --------------- CODE FOR INIT TIMER --------------- */
bool init_timer_irq(tmr_repeat_irq_t* handler){
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
        handler->alarm_done = true;
    } else {
        // --- Timer is already enabled
        sleep_ms(1);
    }    
    handler->init_done = true;
    return handler->init_done;
};


bool enable_repeat_timer_irq(tmr_repeat_irq_t* handler){
    if(!handler->init_done){
        // --- Timer is not initialized
        init_timer_irq(handler);
    }     
    
    // Register the func
    if (!add_repeating_timer_us((int64_t)handler->period_us, handler->func_irq, NULL, handler->timer)) {
        handler->enable_state = false;
    } else {
        handler->enable_state = true;
    }
    return handler->enable_state;
};    


bool disable_repeat_timer_irq(tmr_repeat_irq_t* handler){
    if(!handler->init_done){
        // --- Timer is not initialized
        init_timer_irq(handler);
    }  

    handler->enable_state = !cancel_repeating_timer(handler->timer);
    return !handler->enable_state;
};


bool activate_oneshot_timer_irq(tmr_repeat_irq_t* handler){
    if(!handler->init_done){
        // --- Timer is not initialized
        init_timer_irq(handler);
    }     
    
    // Register the func
    if (!add_alarm_in_us(handler->period_us, handler->func_irq, NULL, handler->timer)) {
        handler->enable_state = true;
    } else {
        handler->enable_state = false;
    }
    return handler->enable_state;
};    
