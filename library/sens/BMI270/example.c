#include <stdio.h>
#include <stdlib.h>

#include "sens/bmi270/bmi270_i2c.h"


int main(){ 
    static i2c_t I2C_USED = {
        .pin_sda = 14,
        .pin_scl = 15,
        .i2c_mod = i2c1,
        .fi2c_khz = 400,
        .init_done = false
    };
    static bmi270_i2c_t sens_device = {
        .i2c_mod = &I2C_USED,
        .en_adv_pwr_mode = false,
        .en_temp_sensor = true,
        .en_gyro_sensor = true,
        .gyro_odr = BMI270_ODR_100,
        .gyro_range = BMI270_GYRO_RANGE_2000,
        .en_acc_sensor = true,
        .acc_odr = BMI270_ODR_100,
        .acc_range = BMI270_ACC_RANGE_4G,    
        .do_noise_performance_opt = false,
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
    BMI270_i2c_init(&sens_device);
    printf("BMI270 available?: %d\n\n", BMI270_i2c_read_id(&sens_device, true));
    sleep_ms(100);    

    //Main Loop for communication
    bmi270_data_t data = BMI270_DATA_DEFAULT;
    while (true){
        printf("Error: %x\n", BMI270_i2c_get_error_register(&sens_device));
        printf("Status: %x\n", BMI270_i2c_get_status_register(&sens_device));
        //BMI270_i2c_get_status_internal_register(&sens_device, true);
        
        //data = BMI270_i2c_get_gyroscope_data(&sens_device);
        //data = BMI270_i2c_get_accelerator_data(&sens_device);
        data = BMI270_i2c_get_all_data(&sens_device);
        printf("Time: %f s, Temp = %f °C\n", data.time, data.temp);
        printf("Gyro data --> x: %f, y: %f, z: %f\n", data.gyro_x, data.gyro_y, data.gyro_z);
        printf("Acc data --> x: %f, y: %f, z: %f\n", data.acc_x, data.acc_y, data.acc_z);
        printf("\n");
        sleep_ms(1000);
    };
    return 0;
}
