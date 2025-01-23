#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "lib/i2c_handler.h"
#include "sens/veml7700.h"


int main(){ 
    veml7700_handler_t setting_device = {
        .i2c_mod = &DEVICE_I2C_DEFAULT,
		.adr = 0x00,
		.gain = VEML7700_GAIN_X1,
		.int_time = VEML7700_INT_100MS,
		.en_pwr_saving = false,
		.use_isr_thres = false
    }

    // --- Init of Serial COM-Port
    stdio_init_all();
	
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    }  

    // Init of device
    scan_i2c_bus_for_device(&setting_device);
    VEML7700_init(&setting_device, true);

    //Main Loop for communication
    uint16_t als = 0;
    uint16_t whv = 0;

    while (true){
        als = VEML7700_get_als_value(&setting_device);
        whv = VEML7700_get_white(&setting_device);
        sleep_ms(250);
    };
}
