#include <stdbool.h>
#include "gpio_callbacks.h"
#include "hardware_io.h"


// ============================== ISR ROUTINES ==============================
void gpio_isr_button_user(uint gpio, uint32_t events, uint8_t gpio_led){
    gpio_put(gpio_led, !gpio_get(gpio_led));
}


void gpio_callbacks(uint gpio, uint32_t events){
    switch(gpio){
        case BTTN_USER_PIN: gpio_isr_button_user(gpio, events);                     break;
		default:			sleep_us(1);											break;
    };
}
