#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hal/clk/pio_clock.h"


#define CLK_PIO_PIN 2
#define CLK_FRQ_VAL	1000


int main(){   
    // Init Phase
    PIO pio = pio0;
    clk_generation_pio_init(pio, CLK_PIO_PIN, CLK_FRQ_VAL, 125000000);
	
	// Infinity Loop
    while (true){
		  sleep_ms(1000);
    }
}
