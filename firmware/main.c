#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware_io.h"

#include "lib/ws2812.h"
#include "lib/usb_handler.h"
#include "src/timer_daq.h"


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


    gpio_init(CLK_TEST_PIN);
    gpio_set_dir(CLK_TEST_PIN, GPIO_OUT);
    gpio_put(CLK_TEST_PIN, false);

    // Init NeoPixel
    gpio_init(NEOPIXEL_PIN);
    gpio_set_dir(NEOPIXEL_PIN, GPIO_OUT);
    gpio_put(NEOPIXEL_PIN, true);

    PIO pio = pio1;
    ws2812_init(pio, NEOPIXEL_PIN, 800000);
    put_pixel_rgb(0, 0, 0);
    sleep_ms(100);
    put_pixel_rgb(led_intensity, led_intensity, led_intensity);
    sleep_ms(1500);
    put_pixel_rgb(0, 0, 0);
}


int main(){
    stdio_init_all();
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
