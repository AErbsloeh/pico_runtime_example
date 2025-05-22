#include "wrapper/led_control.h"


// ======================================== INTERNAL FUNCTIONS ===============================================
uint8_t pin_used = 0;


// ======================================== CALLABLE FUNCTIONS ===============================================
void init_default_led(uint8_t led_pin){
	uint8_t pin_used = led_pin;

	if (PICO_BOARD == "pico2_w" || PICO_BOARD == "pico_w") {
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
	} else {
		gpio_init(pin_used);
		gpio_set_dir(pin_used, GPIO_OUT);
		gpio_put(pin_used, false);
	}
};

bool set_default_led(bool state){
	if (PICO_BOARD == "pico2_w" || PICO_BOARD == "pico_w") {
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
	} else {
		gpio_put(pin_used, state);
	}
	return state;
};

bool get_default_led(void){
	if (PICO_BOARD == "pico2_w" || PICO_BOARD == "pico_w") {
		return cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
	} else {
		return gpio_get(pin_used);
	}
};
