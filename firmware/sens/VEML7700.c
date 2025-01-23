#include "sens/veml7700.h"


bool VEML7700_read_id(veml7700_handler_t *handler){
    uint8_t  buffer[2] = {0x00};
    buffer[0] = 0x07;
    buffer[1] = 0x00;

    i2c_write_blocking(handler->i2c_mod->i2c_mod, VEML7700_ADR, buffer, 1, false);
    while (i2c_read_blocking(handler->i2c_mod->i2c_mod, VEML7700_ADR, buffer, 2, false) == PICO_ERROR_GENERIC);
    uint16_t id = ((buffer[0] << 8) + buffer[1]); 
    printf("%d\n", id);

    return (id == 0xC481) || (id == 0xD481);
}


bool VEML7700_init(veml7700_handler_t *handler){
    if(!handler->i2c_mod->init_done){
        configure_i2c_module(handler->i2c_mod);
    }

    // --- Do init of sensor       
    // Load write command and current user register content to buffer to be written
    uint8_t register_data[3] = {0x00};
    register_data[1] = ((handler->int_time & 0x03) << 6);
    register_data[2] = ((handler->gain & 0x07) << 3) | ((handler->int_time & 0x0C) >> 2);
    
    // Set user register data to be written
    if (handler->en_pwr_saving) {
        register_data[1] &= ~0x01;
    } else {
        register_data[1] |= 0x01; 
    }

    if (handler->use_isr_thres) {
        register_data[1] |= 0x02;
    } else {
        register_data[1] &= ~0x02; 
    }

    // Send user resgister data to sensor
    if (i2c_write_blocking(handler->i2c_mod->i2c_mod, VEML7700_ADR, register_data, 3, false) == PICO_ERROR_GENERIC) {
        handler->init_done = false;        
    }else{
        handler->init_done = true;
    }
    return handler->init_done;
}


void VEML7700_set_power_saving_mode(veml7700_handler_t *handler, uint8_t mode){
    uint8_t  buffer[3] = {0};
    buffer[0] = 0x04;
    buffer[2] = 0x00;
    buffer[1] = ((mode & 0x03) << 1);
    if(handler->en_pwr_saving) {
        buffer[2] |= 0x01;
    } else {
        buffer[2] &= ~0x01;
    }

    i2c_write_blocking(handler->i2c_mod->i2c_mod, VEML7700_ADR, buffer, 3, false);
}


uint16_t VEML7700_get_als_value(veml7700_handler_t *handler){
    uint8_t  buffer[2] = {0x04};

    i2c_write_blocking(handler->i2c_mod->i2c_mod, VEML7700_ADR, buffer, 1, false);
    while (i2c_read_blocking(handler->i2c_mod->i2c_mod, VEML7700_ADR, buffer, 2, false) == PICO_ERROR_GENERIC);
    uint16_t raw_als = ((buffer[0] << 8) + buffer[1]); 
    return raw_als;
}


uint16_t VEML7700_get_white(veml7700_handler_t *handler){
    uint8_t  buffer[2] = {0x05};

    i2c_write_blocking(handler->i2c_mod->i2c_mod, VEML7700_ADR, buffer, 1, false);
    while (i2c_read_blocking(handler->i2c_mod->i2c_mod, VEML7700_ADR, buffer, 2, false) == PICO_ERROR_GENERIC);
    uint16_t raw_white = ((buffer[0] << 8) + buffer[1]); 
    return raw_white;
}
