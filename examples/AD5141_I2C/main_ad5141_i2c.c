#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "lib/i2c_handler.h"
#include "sens/ad5141_i2c.h"


int main(){ 
    ad5141_i2c_t setting_device = {
        .i2c_handler = &DEVICE_I2C_DEFAULT,
        .adr = 0x00,
        .init_done
    }

    // Init of device
    scan_i2c_bus_for_device(&setting_device);
    ad5141_i2c_init(&setting_device, 0);

    //Main Loop for communication
    uint8_t pos = 0;
    while (true){
        ad5141_i2c_define_level(&setting_device, 0, pos);
        pos++;
        sleep_ms(250);
    };
}
