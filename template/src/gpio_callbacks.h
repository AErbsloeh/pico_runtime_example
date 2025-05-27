#ifndef GPIO_CALLBACKS_H_
#define GPIO_CALLBACKS_H_

#include <stdint.h>
#include "pico/types.h"


/*!	Function for running the handler of GPIO ISR callbacks
* \param gpio 	GPIO number which indicates the ISR call
* \param events	Number of events
*/
void irq_gpio_callbacks(uint gpio, uint32_t events);


#endif
