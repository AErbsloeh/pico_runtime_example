#include <stdio.h>
#include <stdlib.h>

#include "poti/ad5142a/ad5142a.h"


int main(){ 
    ad5142a_t setting_device = {
        .i2c_handler = &DEVICE_I2C_DEFAULT,
        .adr = 0x00,
        .init_done
    }

    // --- Init of Serial COM-Port
    stdio_init_all();
	
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    }  

    // Init of device
    scan_i2c_bus_for_device(&setting_device);
    ad5142a_init(&setting_device, 0);

    //Main Loop for communication
    uint8_t pos = 0;
    while (true){
        ad5142a_define_level(&setting_device, 0, pos);
        pos++;
        sleep_ms(250);
    };
}
