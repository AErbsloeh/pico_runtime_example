#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "wrapper/i2c_handler.h"
#include "sens/veml7700.h"


int main(){ 
    // --- Init of Serial COM-Port
    stdio_init_all();
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    };
    sleep_ms(1000);

    // Init of device
    scan_i2c_bus_for_device(&I2C_USED);
    if(VEML7700_init(&VEML7700_DEFAULT_CONFIG)){
        gpio_put(PICO_DEFAULT_LED_PIN, true);
    };
    VEML7700_read_id(&VEML7700_DEFAULT_CONFIG, true);
    sleep_ms(100);    

    //Main Loop for communication
    uint16_t als = 0.0;
    uint16_t whs = 0.0;

    while (true){
        als = VEML7700_get_als_value(&VEML7700_DEFAULT_CONFIG);
        whs = VEML7700_get_white(&VEML7700_DEFAULT_CONFIG);
        printf("... WHS = %d, ALS = %d\n", whs, als);
        sleep_ms(1000);
    };
    return 0;
}
