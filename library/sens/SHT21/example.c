#include <stdio.h>
#include <stdlib.h>

#include "sens/sht21/sht21.h"


int main(){ 
    // --- Init of Serial COM-Port
    stdio_init_all();

    sht21_t sht21_test = {
        .i2c_mod = &DEVICE_I2C_DEFAULT,
        .heater_enable = SHT21_HEATER_OFF,
        .otp_enable = SHT21_DISABLE_OTP,
        .resolution = SHT21_RESOLUTION_12_14,
        .init_done = false
    };
	
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    }  

    // Init of device
    scan_i2c_bus_for_device(&DEVICE_I2C_DEFAULT);
    SHT21_init(&sht21_test);

    //Main Loop for communication
    float hum = 0.0;
    float temp = 0.0;

    while (true){
        hum = SHT21_get_humidity_float(&sht21_test);
        temp = SHT21_get_temperature_float(&sht21_test);
        printf("... RH = %f %%, T = %f K", hum, temp);
        sleep_ms(1000);
    };
}
