#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "lib/i2c_handler.h"
#include "sens/sht21.h"


int main(){ 
    // --- Init of Serial COM-Port
    stdio_init_all();
	
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    }  

    // Init of device
    scan_i2c_bus_for_device(&DEVICE_I2C_DEFAULT);
    SHT21_init(&SHT21_CONFIG_DEFAULT);

    //Main Loop for communication
    float hum = 0.0;
    float temp = 0.0;

    while (true){
        hum = SHT21_get_humidity_float(&SHT21_CONFIG_DEFAULT);
        temp = SHT21_get_temperature_float(&SHT21_CONFIG_DEFAULT);
        printf("... RH = %f %%, T = %f K", hum, temp);
        sleep_ms(1000);
    };
}
