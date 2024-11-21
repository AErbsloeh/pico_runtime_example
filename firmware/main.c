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

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    blink_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);

    // PIO counter program takes 3 more cycles in total than we pass as
    // input (wait for n + 1; mov; jmp)
    pio->txf[sm] = (MCU_CLK / (2 * freq)) - 3;
}


void init_system(uint8_t light_intensity){
    //Init GPIO + IRQ (Low Level)
    gpio_init(BUTTON_BOARD);
    gpio_set_dir(BUTTON_BOARD, GPIO_IN);
    gpio_pull_up(BUTTON_BOARD);
    gpio_set_slew_rate(BUTTON_BOARD, GPIO_SLEW_RATE_SLOW);
    gpio_set_irq_enabled_with_callback(BUTTON_BOARD, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Generating a HW-Clock with PIO
    gpio_init(CLK_TEST_PIN);
    gpio_set_dir(CLK_TEST_PIN, GPIO_OUT);
    gpio_put(CLK_TEST_PIN, false);
    PIO pio_mod0 = pio0;
    uint offset = pio_add_program(pio_mod0, &blink_program);
    blink_pin_forever(pio_mod0, 0, offset, CLK_TEST_PIN, TEST_CLK);

    // Controlling a NeoPixel LED (only on KB2040)
    gpio_init(NEOPIXEL_PIN);
    gpio_set_dir(NEOPIXEL_PIN, GPIO_OUT);
    gpio_put(NEOPIXEL_PIN, true);
    PIO pio_mod1 = pio1;
    ws2812_init(pio_mod1, NEOPIXEL_PIN, 800000);
    put_pixel_rgb(0, 0, 0);
    sleep_ms(100);
    put_pixel_rgb(led_intensity, led_intensity, led_intensity);
    sleep_ms(1500);
    put_pixel_rgb(0, 0, 0);

    // Generating a ISR-TMR
    gpio_init(BOARD_LED_PIN);
    gpio_set_dir(BOARD_LED_PIN, GPIO_OUT);
    gpio_put(BOARD_LED_PIN, false);
    init_timer0_isr(true, -10000000);

    // --- Init of Serial COM-Port
    stdio_init_all();
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

    // Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    }    

    //Main Loop for communication
    while (true){
        process_usb_data(&usb_buffer, CLK_TEST_PIN);
    }
}
