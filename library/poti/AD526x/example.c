#include <stdio.h>
#include <stdlib.h>

#include "poti/ad526x/ad526x.h"


int main(){ 
    static ad526x_t setting_device = {
        .spi_handler = &DEVICE_SPI_DEFAULT,
        .gpio_cs = PICO_DEFAULT_SPI_CSN_PIN,
        .device_mode = false,
        .shutdown = true,
        .init_done = false
    };

    // Init of device
    if(ad526x_init(&setting_device)) 
        prinft("Init of DigPot AD526x done\n");

    //Main Loop for communication
    uint8_t position = 0;
    while (true){
        ad526x_define_output(&setting_device, false, position);
        ad526x_define_output(&setting_device, true, position);
        position++;
        sleep_ms(100);
    };
}
