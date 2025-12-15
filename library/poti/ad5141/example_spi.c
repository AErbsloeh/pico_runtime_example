#include <stdio.h>
#include <stdlib.h>


#include "poti/ad5141_spi/ad5141_spi.h"


int main(){ 
    ad5141_spi_t setting_device = {
        .spi_handler = &DEVICE_SPI_DEFAULT,
        .device_csn = PICO_DEFAULT_SPI_CSN_PIN,
        .init_done
    }

    // Init of device
    ad5141_spi_init(&setting_device);

    //Main Loop for communication
    uint8_t pos = 0;
    while (true){
        ad5141_spi_define_level(&setting_device, 0, pos);
        pos++;
        sleep_ms(250);
    };
}
