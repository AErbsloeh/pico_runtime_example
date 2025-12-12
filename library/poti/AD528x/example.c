#include <stdio.h>
#include <stdlib.h>

#include "poti/ad528x/ad528x.h"


int main(){ 
    static ad528x_t setting_device = {
        .i2c_handler = &DEVICE_I2C_DEFAULT,
        .mode_sel = 0,
        .shutdown = true,
        .gpio_state = false,
        .init_done = false
    };


    // Init of device
    if(ad528x_init(&setting_device)) 
        prinft("Init of MUX ADG1408 done\n");


    //Main Loop for communication
    uint8_t position = 0;
    while (true){
        setting_device.gpio_state = AD528X_GPIO_PIN0;
        ad528x_define_gpio_output(&setting_device);
        sleep_ms(100);

        setting_device.gpio_state = AD528X_GPIO_PIN0 | AD528X_GPIO_PIN1;
        ad528x_define_gpio_output(&setting_device); 
        sleep_ms(100);

        setting_device.gpio_state = AD528X_GPIO_NONE;
        ad528x_define_gpio_output(&setting_device);
        sleep_ms(100);

        ad528x_define_output(&setting_device, false, position);
        ad528x_define_output(&setting_device, true, position);
        position++;
        sleep_ms(100);
    };
}
