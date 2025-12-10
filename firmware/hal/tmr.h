#ifndef HAL_TMR_H_
#define HAL_TMR_H_


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"


// ========================================= INCLUDES ===============================================
extern bool alarm_done;

/*! \brief Struct definition for handling an ISR as timer
 * \param timer              Repeating timer handler
 * \param irq_number         32-bit integer as timer IRQ number
 * \param period_us          32-bit integer as timer period in microseconds (<0 repeat to every period, >0 wait period before start)
 * \param alarm_done         Boolean for the timer ISR
 * \param enable_state       Boolean for the timer enable state
 * \param init_done          Boolean for the timer initialization state
 * \param func_irq           Function pointer for the timer ISR
*/
typedef struct {
    repeating_timer_t *timer;
    uint32_t irq_number;
    int64_t period_us;
    bool alarm_done;    
    bool enable_state;
    bool init_done;
    void (*func_irq);
} tmr_repeat_irq_t;


// ========================================= EXAMPLE ===============================================
bool tmr_irq_routine_example(repeating_timer_t *rt);
static repeating_timer_t tmr_example;
static tmr_repeat_irq_t tmr0_example = {
    .timer = &tmr_example,
    .irq_number = 0,
    .period_us = 250000,
    .alarm_done = false,
    .enable_state = true,
    .init_done = false,
    .func_irq = tmr_irq_routine_example
};


// ======================================== FUNCTIONS ===============================================
/*! \brief Function for initialization of a repeating timer using an interrupt service routine (ISR) on Pico
* \param handler      Struct handler for the repeating timer
* \return             Boolean for done initialisation of timer ISR
*/
bool init_timer_irq(tmr_repeat_irq_t* handler);


/*! \brief Enabling an repeating timer using an interrupt service routine (ISR) on Pico
* \param handler      Struct handler for the repeating timer
* \return             Boolean for done initialisation of timer ISR
*/
bool enable_repeat_timer_irq(tmr_repeat_irq_t* handler);


/*! \brief Disabling an repeating timer using an interrupt service routine (ISR) on Pico
* \param handler      Struct handler for the repeating timer
* \return             Boolean for done initialisation of timer ISR
*/
bool disable_repeat_timer_irq(tmr_repeat_irq_t* handler);


/*! \brief Enabling an repeating timer using an interrupt service routine (ISR) on Pico
* \param handler      Struct handler for the repeating timer
* \return             Boolean for done initialisation of timer ISR
*/
bool activate_oneshot_timer_irq(tmr_repeat_irq_t* handler);


#endif
