#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "hal/spi/spi.h"
#include "mux/adgs1208/adgs1208.h"


int main(){ 
    adgs1208_t setting_device = {
        .spi_handler = &DEVICE_SPI_DEFAULT,
        .gpio_num_csn = PICO_DEFAULT_SPI_CSN_PIN,
        .gpio_num_rstn = 5,
        .use_rstn_hw = false,
        .gpio_1_state = false,
        .gpio_2_state = true,
        .gpio_3_state = false,
        .gpio_4_state = true,
        .init_done = false
    }

    // Init of device
    adgs1208_init(&setting_device);

    //Main Loop for communication
    uint8_t pos[1] = {2};

    while (true){
        adgs1208_define_output(&setting_device, 0, &pos);
        pos[0]++;
        sleep_ms(250);
    };
}
