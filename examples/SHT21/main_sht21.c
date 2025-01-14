#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "lib/i2c_handler.h"
#include "sens/SHT21.h"


int main(){ 
    static sht21_handler_t sht21_device = {
        .i2c_mod = &DEVICE_I2C_DEFAULT,
        .heater_enable = SHT21_HEATER_OFF,
        .otp_enable = SHT21_DISABLE_OTP,
        .resolution = SHT21_RESOLUTION_12_14,
        .init_done = false
    };

    // Init of device
    scan_i2c_bus_for_device(&DEVICE_I2C_DEFAULT);
    SHT21_init(&sht21_device, 0);

    //Main Loop for communication
    float hum = 0.0;
    float temp = 0.0;

    while (true){
        hum = SHT21_get_humidity_float(&sht21_device);
        temp = SHT21_get_temperature_float(&sht21_device);
        printf("... RH = %f \%, T = %f °C", hum, temp);
        sleep_ms(1000);
    };
}
