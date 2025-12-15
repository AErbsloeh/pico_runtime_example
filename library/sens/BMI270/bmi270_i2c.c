#include "sens/bmi270/bmi270_i2c.h"
#include "sens/bmi270/bmi270_config.h"
#include "hardware/gpio.h"
#include <stdio.h>


// ======================================== INTERNAL READ/WRITE FUNCTIONS ===============================================
bool BMI270_i2c_write_byte(bmi270_i2c_t *handler, uint8_t command, uint8_t data){
    uint8_t buffer_tx[2] = {command};
    buffer_tx[1] = data;

    i2c_write_blocking(handler->i2c_mod->i2c_mod, BMI270_I2C_ADR, buffer_tx, sizeof(buffer_tx), false);
    sleep_us(50);
    return true;
}


bool BMI270_i2c_write_block(bmi270_i2c_t *handler, uint8_t data[], uint8_t size){
    i2c_write_blocking(handler->i2c_mod->i2c_mod, BMI270_I2C_ADR, data, size, false);
    return true;
}


uint64_t BMI270_i2c_read_data(bmi270_i2c_t *handler, uint8_t command, uint8_t buffer_rx[], uint8_t size){
    uint8_t buffer_tx[1] = {command};

    i2c_write_blocking(handler->i2c_mod->i2c_mod, BMI270_I2C_ADR, buffer_tx, sizeof(buffer_tx), true);
    sleep_us(10);
    while (i2c_read_blocking(handler->i2c_mod->i2c_mod, BMI270_I2C_ADR, buffer_rx, size, false) == PICO_ERROR_GENERIC){
        sleep_ms(2);
    }; 

    uint64_t raw_data = 0;
    for(uint8_t idx = 0; idx < size; idx++){
        raw_data |= buffer_rx[idx] << 8*idx;
    }
    return raw_data;
}


// ======================================== FUNCTIONS ===============================================
bool BMI270_i2c_read_id(bmi270_i2c_t *handler, bool print_id){
    uint8_t data[1] = {0x00};
    uint8_t id = BMI270_i2c_read_data(handler, 0x00, data, 1);
    if(print_id){
        printf("ID read from BMI270: %x\n", id);
    }    
    return (id && 0x24);
};


bool BMI270_i2c_soft_reset(bmi270_i2c_t *handler){
    uint8_t data = 0xB6;
    BMI270_i2c_write_byte(handler, 0x7E, data);
    handler->init_done = false;
    return true;
}


bool BMI270_i2c_init(bmi270_i2c_t *handler){
    init_i2c_module(handler->i2c_mod);
    
    if(!BMI270_i2c_read_id(handler, false)){
		handler->init_done = false; 
	} else {		
		if(BMI270_i2c_get_status_internal_register(handler, false) != 0x01){
			// --- Step #1: Soft reset
			BMI270_i2c_soft_reset(handler);
			sleep_ms(2);

			// --- Step #2: Writing to PWR_CONF Register (FIFO disabled, fast power up enabled)
			BMI270_i2c_write_byte(handler, 0x7C, 0x00);
			sleep_us(500);

			// --- Step #3: Start Initialization
			BMI270_i2c_write_byte(handler, 0x59, 0x00);

			// --- Step #4: Laod config file into device
			for (uint16_t idx = 0; idx < 256; idx++) {
				BMI270_i2c_write_byte(handler, 0x5B, 0x00);
				BMI270_i2c_write_byte(handler, 0x5C, idx);

				uint8_t buffer[33] = {0x5E};    
				for(uint8_t pos = 0; pos < 32; pos++){
					buffer[pos + 1] = bmi270_config_file[32*idx + pos];
				}     
				BMI270_i2c_write_block(handler, buffer, 33);
				sleep_us(20);
			};
			sleep_ms(1);

			// --- Step #5: Initialization done
			BMI270_i2c_write_byte(handler, 0x59, 0x01);
			sleep_ms(1);

			// --- Step #6: Writing to PWR_CTRL Register
			uint8_t data = 0x00 | (handler->en_temp_sensor << 3) | (handler->en_acc_sensor << 2) | (handler->en_gyro_sensor << 1);
			BMI270_i2c_write_byte(handler, 0x7D, data);
			sleep_ms(1);

			// --- Step #7: Writing to PWR_CONF Register (FIFO disabled, fast power up enabled)
			data = 0x00 | (handler->en_adv_pwr_mode << 0);
			BMI270_i2c_write_byte(handler, 0x7C, data);
			sleep_ms(10);

			// --- Step #8: Settings
			BMI270_i2c_set_accelerator_settings(handler);
			BMI270_i2c_set_gyroscope_settings(handler);
		};
		handler->init_done = true; 
	};
    return handler->init_done;
}


bool BMI270_i2c_set_gyroscope_settings(bmi270_i2c_t *handler){
    // GYRO CONF
    uint8_t data = (handler->do_noise_performance_opt << 6) | (handler->gyro_odr & 0x0F);
    BMI270_i2c_write_byte(handler, 0x42, data);
    sleep_us(20);

    // GYRO RANGE
    data = (handler->gyro_range & 0x07);
    BMI270_i2c_write_byte(handler, 0x43, data);
    sleep_us(20);

    return true;
}


bool BMI270_i2c_set_accelerator_settings(bmi270_i2c_t *handler){
    // ACC CONF
    uint8_t data = (handler->acc_odr & 0x0F);
    BMI270_i2c_write_byte(handler, 0x44, data);
    sleep_us(20);

    // ACC RANGE
    data = (handler->acc_range & 0x07);
    BMI270_i2c_write_byte(handler, 0x41, data);
    sleep_us(20);

    return true; 
}


uint8_t BMI270_i2c_get_error_register(bmi270_i2c_t *handler){
    uint8_t data[1] = {0x02};
    return BMI270_i2c_read_data(handler, 0x02, data, 1);
}


uint8_t BMI270_i2c_get_status_register(bmi270_i2c_t *handler){
    uint8_t data[1] = {0x03};
    return BMI270_i2c_read_data(handler, 0x03, data, 1);
}


uint8_t BMI270_i2c_get_status_internal_register(bmi270_i2c_t *handler, bool print_status){
    uint8_t data[1] = {0x21};
    uint8_t status = BMI270_i2c_read_data(handler, 0x21, data, 1) & 0x0F;
    if(print_status){
        switch(status){
            case(0x00): printf("BMI270 not initialized\n"); break;
            case(0x01): printf("BMI270 is initialized\n"); break;
            case(0x02): printf("BMI270 has an initialization error\n"); break;
            case(0x03): printf("BMI270 has an invalid driver\n"); break;
            case(0x04): printf("BMI270 has stopped\n"); break;
        }
    }
    return status;    
}


uint8_t BMI270_i2c_get_power_register(bmi270_i2c_t *handler){
    uint8_t data[1] = {0x7D};
    return BMI270_i2c_read_data(handler, 0x7D, data, 1);
}


double BMI270_i2c_get_temperature(bmi270_i2c_t *handler){
    if(handler->en_temp_sensor){
        uint8_t data[2] = {0x22};
        int16_t temp_raw = BMI270_i2c_read_data(handler, 0x22, data, 2);
        return temp_raw * 0.001952594 + 23.0;
    } else {
        return 0.0;
    };    
}


double BMI270_i2c_get_sensor_time(bmi270_i2c_t *handler){
    uint8_t data[3] = {0x18};
    double time_raw = BMI270_i2c_read_data(handler, 0x18, data, 3) * 39.0625e-6;
    return time_raw;
}


double BMI270_get_scale_gyroscope(bmi270_i2c_t *handler){
    double value = 0.0;
    switch(handler->acc_range){
        case(BMI270_GYRO_RANGE_2000):   value = 2000.0; break;
        case(BMI270_GYRO_RANGE_1000):   value = 1000.0; break;
        case(BMI270_GYRO_RANGE_500):    value = 500.0;  break;
        case(BMI270_GYRO_RANGE_250):    value = 250.0;  break;
        case(BMI270_GYRO_RANGE_125):    value = 125.0;  break;
    };
    return value / 32768;
}


bmi270_data_t BMI270_i2c_get_gyroscope_data(bmi270_i2c_t *handler){
    if(handler->en_gyro_sensor && handler->init_done){
        uint8_t data[6] = {0x12};
        BMI270_i2c_read_data(handler, 0x12, data, 6);

        double z = (int16_t)((data[5] << 8) | (data[4] << 0)) * BMI270_get_scale_gyroscope(handler);
        double y = (int16_t)((data[3] << 8) | (data[2] << 0)) * BMI270_get_scale_gyroscope(handler);
        double x = (int16_t)((data[1] << 8) | (data[0] << 0)) * BMI270_get_scale_gyroscope(handler);

        bmi270_data_t data0 = {
            .temp = BMI270_i2c_get_temperature(handler),
            .time =  BMI270_i2c_get_sensor_time(handler),
            .gyro_x = x,
            .gyro_y = y,
            .gyro_z = z,
            .acc_z = 0,
            .acc_y = 0,
            .acc_x = 0
        };
        return data0;
    } else {
        return BMI270_DATA_DEFAULT;
    };
}    


double BMI270_get_scale_accelerator(bmi270_i2c_t *handler){
    double value = 0.0;
    switch(handler->acc_range){
        case(BMI270_ACC_RANGE_16G):     value = 16.0;   break;
        case(BMI270_ACC_RANGE_8G):      value = 8.0;    break;
        case(BMI270_ACC_RANGE_4G):      value = 4.0;    break;
        case(BMI270_ACC_RANGE_2G):      value = 2.0;    break;
    };
    return value / 32768;
}


bmi270_data_t BMI270_i2c_get_accelerator_data(bmi270_i2c_t *handler){
    if(handler->en_acc_sensor && handler->init_done){
        uint8_t data[6] = {0x0C};
        BMI270_i2c_read_data(handler, 0x0C, data, 6);

        double z = (int16_t)((data[5] << 8) | (data[4] << 0)) * BMI270_get_scale_accelerator(handler);
        double y = (int16_t)((data[3] << 8) | (data[2] << 0)) * BMI270_get_scale_accelerator(handler);
        double x = (int16_t)((data[1] << 8) | (data[0] << 0)) * BMI270_get_scale_accelerator(handler);

        bmi270_data_t data0 = {
            .temp = BMI270_i2c_get_temperature(handler),
            .time =  BMI270_i2c_get_sensor_time(handler),
            .gyro_x = 0,
            .gyro_y = 0,
            .gyro_z = 0,
            .acc_z = z,
            .acc_y = y,
            .acc_x = x
        };
        return data0;
    } else {
        return BMI270_DATA_DEFAULT;
    };
}


bmi270_data_t BMI270_i2c_get_all_data(bmi270_i2c_t *handler){
    if(handler->en_acc_sensor && handler->en_gyro_sensor && handler->init_done){
        uint8_t data[15] = {0x0C};
        BMI270_i2c_read_data(handler, 0x0C, data, 15);

        double a_x = (int16_t)((data[1] << 8) | (data[0] << 0)) * BMI270_get_scale_accelerator(handler);
        double a_y = (int16_t)((data[3] << 8) | (data[2] << 0)) * BMI270_get_scale_accelerator(handler);
        double a_z = (int16_t)((data[5] << 8) | (data[4] << 0)) * BMI270_get_scale_accelerator(handler);
        double g_x = (int16_t)((data[7] << 8) | (data[6] << 0)) * BMI270_get_scale_gyroscope(handler);
        double g_y = (int16_t)((data[9] << 8) | (data[8] << 0)) * BMI270_get_scale_gyroscope(handler);
        double g_z = (int16_t)((data[11] << 8) | (data[10] << 0)) * BMI270_get_scale_gyroscope(handler);
        double time = (double)(((data[14] << 16) | (data[13] << 8) | (data[12] << 0)) * 39.0625e-6);

        bmi270_data_t data0 = {
            .temp = BMI270_i2c_get_temperature(handler),
            .time = time,
            .gyro_x = g_x,
            .gyro_y = g_y,
            .gyro_z = g_z,
            .acc_z = a_z,
            .acc_y = a_y,
            .acc_x = a_x
        };
        
        return data0;
    } else {
        return BMI270_DATA_DEFAULT;
    };    
}
