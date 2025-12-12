#include <stdio.h>
#include <stdlib.h>

#include "sens/veml7700/veml7700.h"


int main(){ 
    // --- Init of Serial COM-Port
    stdio_init_all();
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    };
    sleep_ms(1000);

    static veml7700_t veml_test = {
        .i2c_mod = &DEVICE_I2C_DEFAULT,
        .gain = VEML7700_GAIN_X1,
        .int_time = VEML7700_INT_100MS,
        .en_device = true,
        .use_isr_thres = false,
        .init_done = false
    };

    // Init of device
    scan_i2c_bus_for_device(&I2C_USED);
    if(VEML7700_init(&veml_test)){
        gpio_put(PICO_DEFAULT_LED_PIN, true);
    };
    VEML7700_read_id(&veml_test, true);
    sleep_ms(100);    

    //Main Loop for communication
    uint16_t als = 0.0;
    uint16_t whs = 0.0;

    while (true){
        als = VEML7700_get_als_value(&veml_test);
        whs = VEML7700_get_white(&veml_test);
        printf("... WHS = %d, ALS = %d\n", whs, als);
        sleep_ms(1000);
    };
    return 0;
}
