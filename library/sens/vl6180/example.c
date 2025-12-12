#include <stdio.h>
#include <stdlib.h>

#include "sens/vl6180.h"


int main(){ 
    static i2c_device_handler_t I2C_USED = {
        .pin_sda = 14,
        .pin_scl = 15,
        .i2c_mod = i2c1,
        .fi2c_khz = 400,
        .init_done = false
    };
    static vl6180_t sens_device = {
        .i2c_mod = &I2C_USED,
        .max_convergence_ms = 63,
        .init_done = false
    };


    // --- Init of Serial COM-Port
    stdio_init_all();
	// Wait until USB is connected
    while(!stdio_usb_connected()){
        sleep_ms(10);
    };
    sleep_ms(1000);

    // Init of device
    scan_i2c_bus_for_device(&I2C_USED);
    printf("Device VL6180 available? --> %x\n", VL6180_read_id(&sens_device, true));

    VL6180_init(&sens_device);
    sleep_ms(100);

    //Main Loop for communication
    bool led_state = false;
    while (true){
        VL6180_start_single_measurement(&sens_device);
        printf("Range error: 0x0%x\n", VL6180_get_range_error(&sens_device));
        printf("Range Error GPIO: 0x0%x\n", VL6180_get_range_error_isr(&sens_device));
        printf("Measured range: %d mm\n", VL6180_get_range_value(&sens_device));
        printf("\n");
        sleep_ms(1000);
    };
    return 0;
}
