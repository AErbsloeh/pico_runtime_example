#include "sens/veml7700/veml7700.h"
#include <stdio.h>


// ======================================== FUNCTIONS ===============================================
uint16_t VEML7700_read_data(veml7700_t *handler, uint8_t command){
    uint8_t buffer_tx[1] = {command};
    uint8_t buffer_rx[2] = {command};
    construct_i2c_read_data(handler->i2c_mod, VEML7700_ADR, buffer_tx, 1, buffer_rx, 2);
    uint16_t raw_data = ((buffer_rx[1] << 8) + buffer_rx[0]); 
    return raw_data;
}


bool VEML7700_read_id(veml7700_t *handler, bool print_id){
    uint16_t id = VEML7700_read_data(handler, 0x07);
    if(print_id){
        printf("Read ID of VEML: %x\n", id);
    };
    return (id == 0xC481) || (id == 0xD481);
}


uint16_t VEML7700_get_als_value(veml7700_t *handler){
    if(handler->init_done){
        return VEML7700_read_data(handler, 0x04); 
    } else {
        return 0;
    }
}


uint16_t VEML7700_get_white(veml7700_t *handler){
    if(handler->init_done){
        return VEML7700_read_data(handler, 0x05); 
    } else {
        return 0;
    }
    
}


bool VEML7700_init(veml7700_t *handler){
    init_i2c_module(handler->i2c_mod);
      
    if(!VEML7700_read_id(handler, false)){
        return false;
    } else {
        uint8_t buffer[3] = {0x00};
        buffer[1] = ((handler->gain & 0x07) << 3) | ((handler->int_time & 0x0C) >> 2);
        buffer[2] = ((handler->int_time & 0x03) << 6);
        
        if (handler->en_device) {
            buffer[2] &= ~0x01;
        } else {
            buffer[2] |= 0x01; 
        }

        if (handler->use_isr_thres) {
            buffer[2] |= 0x02;
        } else {
            buffer[2] &= ~0x02; 
        }

        // Send user resgister data to sensor
        construct_i2c_write_data(handler->i2c_mod, VEML7700_ADR, buffer, 3);
        handler->init_done = true;
        return true;
    };
}
