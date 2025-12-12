#include "hal/led/led.h"
#include "hardware/gpio.h"
#ifdef PICO_CYW43_SUPPORTED
    #include "pico/cyw43_arch.h"
#endif

// ======================================== INTERNAL FUNCTIONS ===============================================
uint8_t pin_used = 0;


// ======================================== CALLABLE FUNCTIONS ===============================================
void init_default_led(uint8_t led_pin){
	#ifdef PICO_CYW43_SUPPORTED
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
		pin_used = CYW43_WL_GPIO_LED_PIN;
	#else
		pin_used = led_pin;
		gpio_init(pin_used);
		gpio_set_dir(pin_used, GPIO_OUT);
		gpio_put(pin_used, false);
	#endif
};


bool set_default_led(bool state){
	#ifdef PICO_CYW43_SUPPORTED 
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
	#else
		gpio_put(pin_used, state);
	#endif
	return state;
};


bool get_default_led(void){
	#ifdef PICO_CYW43_SUPPORTED
		return cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
	#else
		return gpio_get(pin_used);
	#endif
};


bool toggle_default_led(void){
	set_default_led(!get_default_led());
	return get_default_led();
};
