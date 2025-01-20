#include "hardware/gpio.h"
#include "hardware/clocks.h"

#include "hardware_io.h"
#include "lib/ws2812.h"
#include "lib/blink.pio.h"
#include "lib/usb_handler.h"
#include "lib/timer_daq.h"


void gpio_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    printf("GPIO IRQ done");
}


void init_system(uint8_t light_intensity){
    //Init GPIO + IRQ (Low Level)
    gpio_init(BUTTON_BOARD);
    gpio_set_dir(BUTTON_BOARD, GPIO_IN);
    gpio_pull_up(BUTTON_BOARD);
    gpio_set_slew_rate(BUTTON_BOARD, GPIO_SLEW_RATE_SLOW);
    gpio_set_irq_enabled_with_callback(BUTTON_BOARD, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);


    // Generating a ISR-TMR
    gpio_init(BOARD_LED_PIN);
    gpio_set_dir(BOARD_LED_PIN, GPIO_OUT);
    gpio_put(BOARD_LED_PIN, false);
    init_timer0_isr(true, -10000000);

    // --- Init of Serial COM-Port
    stdio_init_all();
	
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    }  
}


int main(){   
    // Init Phase 
    init_system(128);

    static char data[USB_FIFO_SIZE] = {0};
    static usb_fifo_buffer usb_buffer = {
        .ready = false,
        .length = USB_FIFO_SIZE,
        .position = USB_FIFO_SIZE-1,
        .data = data
    };  

    //Main Loop for communication
    while (true){
        process_usb_data(&usb_buffer, CLK_TEST_PIN);
    }
}
