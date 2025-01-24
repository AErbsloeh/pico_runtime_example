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
		.en_device = false,
		.use_isr_thres = false
    }

    // --- Init of Serial COM-Port
    stdio_init_all();
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    };
    sleep_ms(1000);

    // Init of device
    scan_i2c_bus_for_device(&I2C_USED);
    if(VEML7700_init(&sens_device)){
        gpio_put(PICO_DEFAULT_LED_PIN, true);
    };
    VEML7700_read_id(&sens_device, true);
    sleep_ms(100);    

    //Main Loop for communication
    uint16_t als = 0.0;
    uint16_t whs = 0.0;

    while (true){
        als = VEML7700_get_als_value(&sens_device);
        whs = VEML7700_get_white(&sens_device);
        printf("... WHS = %d, ALS = %d\n", whs, als);
        sleep_ms(1000);
    };
    return 0;
}
