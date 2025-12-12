#include <stdio.h>
#include <stdlib.h>

#include "sens/adxl345/adxl345_i2c.h"


int main(){ 
    adxl345_i2c_t setting_device = {
        .i2c_mod = &DEVICE_I2C_DEFAULT,
        .offset_x = 0.0,
		.offset_y = 0.0,
		.offset_z = 0.0,
        .init_done = false
    }

    // --- Init of Serial COM-Port
    stdio_init_all();
	
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    }  

    // Init of device
    scan_i2c_bus_for_device(&setting_device);
    if(ADXL345_init(&setting_device))
		prinft("Init of ADXL345 done!");

    //Main Loop for communication
    float acc_x = 0.0, acc_y = 0.0, acc_z = 0.0;
    while (true){
        ADXL345_get_acceleration(&setting_device, &acc_x, &acc_y, &acc_z);
		printf("x=%x, x=%x, x=%x\n", acc_x, acc_y, acc_z);
        sleep_ms(250);
    };
}
