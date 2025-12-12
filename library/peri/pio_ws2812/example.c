#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "peri/pio_ws2812/ws2812.pio.h"


#define NEOPIXEL_PIN 17


int main(){   
    // Init Phase 
    // Controlling a NeoPixel LED (only on KB2040)
    PIO pio_mod1 = pio1;
    ws2812_init(pio_mod1, NEOPIXEL_PIN, 800000);
    
	put_pixel_rgb(0, 0, 0);
    sleep_ms(100);
    

    //Main Loop for communication
	uint_8t red_val = 0;
	uint_8t gre_val = 0;
	uint_8t blu_val = 0;
    while (true){
        put_pixel_rgb(red_val, gre_val, blu_val);
		sleep_ms(1000);
				
		if(red_val == 255) {
			red_val = 0;
			gre_val++;
			if(gre_val == 255) {
					gre_val = 0;
					blu_val++;
			}
		} else { 
			red_val++;
		}
    }
}
