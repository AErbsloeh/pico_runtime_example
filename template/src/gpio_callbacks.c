#include "src/gpio_callbacks.h"
#include "hardware_io.h"


// ============================== ISR ROUTINES ==============================
void irq_gpio_button_user(uint gpio, uint32_t events, uint8_t gpio_led){
    gpio_put(gpio_led, !gpio_get(gpio_led));
}


void irq_gpio_callbacks(uint gpio, uint32_t events){
    switch(gpio){
        case BUTTON_BOARD:  irq_gpio_button_user(gpio, events, LED_TEST_DEFAULT);   break;
		default:			sleep_us(1);											break;
    };
}
