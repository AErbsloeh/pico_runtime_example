#include <stdio.h>
#include "pico/stdlib.h"

/*!
    \param rt
*/
bool process_timer0_irq(repeating_timer_t *rt);


/*!
    \param rt
*/
bool process_timer1_irq(repeating_timer_t *rt);


/*!
    \param enable_timer
    \param period_callback
*/
void init_timer0_isr(bool enable_timer, int32_t period_callback);


/*!
    \param enable_timer
    \param period_callback
*/
void init_timer1_isr(bool enable_timer, int32_t period_callback);
