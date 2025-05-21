#ifndef TIMER_DAQ_H
#define TIMER_DAQ_H


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware_io.h"


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
    uint32_t period_us;
    bool alarm_done;    
    bool enable_state;
    bool init_done;
    void (*func_irq);
} tmr_isr_handler_t;


// ========================================= EXAMPLE ===============================================
bool tmr_irq_routine_example(repeating_timer_t *rt);
static repeating_timer_t tmr_example;
static tmr_isr_handler_t tmr0_example = {
    .timer = &tmr_example,
    .irq_number = 0,
    .period_us = 250000,
    .alarm_done = false,
    .enable_state = true,
    .init_done = false,
    .func_irq = tmr_irq_routine_example
};



// ======================================== FUNCTIONS ===============================================
/*! \brief for initialisating an repeating timer using an interrupt service routine (ISR) on Pico
    \param handler      Struct handler for the repeating timer
    \return             Boolean for done initialisation of timer ISR
*/
bool init_timer_isr(tmr_isr_handler_t* handler);


/*! \brief Function for processing the timer0 interrupt service routine (ISR)
    \param rt   Struct handler for the repeating timer
    \return     Boolean if processing of ISR routine is done
*/
bool process_timer_irq(repeating_timer_t *rt);

#endif
