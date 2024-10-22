#include <stdio.h>
#include "pico/stdlib.h"


static repeating_timer_t timer0;
static volatile bool alarm_done;

bool process_timer_irq(repeating_timer_t *rt);
static void init_timer_irq_alarm(void);
void init_timer_isr(bool enable_timer);