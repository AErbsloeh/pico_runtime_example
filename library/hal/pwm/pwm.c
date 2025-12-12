#include "hal/pwm/pwm.h"    
#include "hardware/pwm.h"


bool pwm_irq_routine_example(struct repeating_timer *rt) {
    return true;
}


bool pwm_init_done_flag = false;

static void pwm_init_irq_alarm(void) {
    pwm_init_done_flag = true;
}


bool init_pwm_irq(pwm_t *handler) {
    if(handler->enable_state && !irq_is_enabled(handler->irq_number)) {
        gpio_set_function(handler->gpio, GPIO_FUNC_PWM);
        handler->slice = pwm_gpio_to_slice_num(handler->gpio);

        pwm_set_clkdiv(handler->slice, handler->clk_div);
        pwm_set_wrap(handler->slice, handler->wrap);

        if(handler->use_irq){
            pwm_clear_irq(handler->slice);
            pwm_set_irq_enabled(handler->slice, true);

            irq_set_exclusive_handler(handler->irq_number, pwm_init_irq_alarm);
            irq_set_enabled(handler->irq_number, true);
            pwm_init_done_flag = false;

            while(!pwm_init_done_flag){
                sleep_ms(1);
            }
            handler->alarm_done = true;
        }

    } else {
        sleep_ms(1);
    }
    handler->init_done = true;
    return handler->init_done;
}


bool enable_pwm(pwm_t *handler){
    if(!handler->init_done)
        init_pwm_irq(handler);

    pwm_set_gpio_level(handler->gpio, handler->level);
    pwm_set_enabled(handler->slice, true);

    handler->enable_state = true;
    return handler->enable_state;
}


bool disable_pwm(pwm_t *handler){
    if(!handler->init_done)
        init_pwm_irq(handler);

    pwm_set_enabled(handler->slice, false);

    handler->enable_state = false;
    return !handler->enable_state;
}


bool pwm_set_level(pwm_t *handler, uint16_t level){
    if(!handler->init_done)
        init_pwm_irq(handler);

    handler->level = level;
    pwm_set_gpio_level(handler->gpio, level);
    return true;
}


bool pwm_update_duty_cycle(pwm_t *handler, uint16_t duty_cycle) {
    if (!handler->init_done)
        init_pwm_irq(handler);

    handler->level = duty_cycle;
    pwm_set_gpio_level(handler->gpio, duty_cycle);

    return true;
}
