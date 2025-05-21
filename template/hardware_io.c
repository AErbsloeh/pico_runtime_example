#include "hardware_io.h"

// ======================================== FUNCTIONS ===============================================
void init_default_led(void){
	if (PICO_BOARD == "pico2_w" || PICO_BOARD == "pico_w") {
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
	} else {
		gpio_init(LED_TEST_DEFAULT);
		gpio_set_dir(LED_TEST_DEFAULT, GPIO_OUT);
		gpio_put(LED_TEST_DEFAULT, false);
	}
}

bool set_default_led(bool state){
	if (PICO_BOARD == "pico2_w" || PICO_BOARD == "pico_w") {
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
	} else {
		gpio_put(LED_TEST_DEFAULT, state);
	}
	return state;
}

bool get_default_led(void){
	if (PICO_BOARD == "pico2_w" || PICO_BOARD == "pico_w") {
		return cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
	} else {
		return gpio_get(LED_TEST_DEFAULT);
	}
}


// ==================== DEVICE DEFINITION =====================

// --- USB PROTOCOL
char data_usb[USB_FIFO_SIZE] = {0};
usb_fifo_buffer usb_buffer = {
	.ready = false,
	.length = USB_FIFO_SIZE,
	.position = USB_FIFO_SIZE-1,
	.data = data_usb
};  
