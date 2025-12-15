#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "hal/spi/spi.h"
#include "adc/ads8881/ads8881.h"


int main(){ 
    static ads8881_t setting_device = {
        .spi_handler = &DEVICE_SPI_DEFAULT,
        .gpio_num_csn = 1,
		.gpio_num_conv = 0,
		.mode = ADS8881_THREE_WIRE_W_BUSY_IND,
        .init_done = false
    };

    // Init of device
    ads8881_init(&setting_device);

    //Main Loop for communication
    uint8_t pos[1] = {0};
    while (true){
        sleep_ms(250);
    };
}
