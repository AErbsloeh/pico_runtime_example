#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

// TODO: Implement code


// pwm_wrap_callback_array: calback array per slice
typedef struct {
    void (*gpio_callback_array[29])(void);
    void (*pwm_wrap_callback_array[8])(void);
    bool init_done;
} main_irq_handler_t;

//The main IRQ Handler for the entire Core
extern main_irq_handler_t main_irq_handler;

void construct_irq_add_gpio(uint8_t pin, uint32_t event_mask, void (*callback)(void));

void construct_irq_add_pwm_wrap(uint8_t pin, void (*callback)(void));

bool main_irq_init(main_irq_handler_t* handler);

main_irq_handler_t* get_main_irq_handler(void);

#endif