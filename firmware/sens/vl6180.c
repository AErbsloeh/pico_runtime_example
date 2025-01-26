#include "sens/vl6180.h"


bool VL6180_write_byte(vl6180_handler_t *handler, uint16_t command, uint8_t data){
    uint8_t buffer_tx[2] = {0};
    buffer_tx[0] = (command & 0xFF00) >> 8;
    buffer_tx[1] = (command & 0x00FF) >> 0;

    i2c_write_blocking(handler->i2c_mod->i2c_mod, VL6180_I2C_ADR, buffer_tx, 2, false);
    sleep_us(50);
    return true;
}


uint64_t VL6180_read_data(vl6180_handler_t *handler, uint16_t command, uint8_t buffer_rx[], uint8_t size){
    uint8_t buffer_tx[2] = {0};
    buffer_tx[0] = (command & 0xFF00) >> 8;
    buffer_tx[1] = (command & 0x00FF) >> 0;

    i2c_write_blocking(handler->i2c_mod->i2c_mod, VL6180_I2C_ADR, buffer_tx, 2, true);
    sleep_us(10);
    while (i2c_read_blocking(handler->i2c_mod->i2c_mod, VL6180_I2C_ADR, buffer_rx, size, false) == PICO_ERROR_GENERIC){
        sleep_ms(2);
    }; 

    uint64_t raw_data = 0;
    for(uint8_t idx = 0; idx < size; idx++){
        raw_data |= buffer_rx[idx] << 8*idx;
    }
    return raw_data;
}


bool VL6180_read_id(vl6180_handler_t *handler, bool print_id){
    uint8_t buffer[1] = {0x00};
    VL6180_read_data(handler, 0x0000, buffer, 1);
    if(print_id){
        printf("Get ID of VL6180: %x\n", buffer[0]);
    }
    return buffer[0] && 0xB4;
}


bool VL6180_soft_reset(vl6180_handler_t *handler){
    VL6180_write_byte(handler, 0x0016, 0x01);
    return true;
}


bool VL6180_disable_reset(vl6180_handler_t *handler){
    VL6180_write_byte(handler, 0x0016, 0x00);
    return true;
}


bool VL6180_init(vl6180_handler_t *handler){
    // Do soft reset
    VL6180_soft_reset(handler);
    sleep_ms(1);
    VL6180_disable_reset(handler);
    sleep_ms(1);

    // Disable GPIO0 and GPIO1
    VL6180_write_byte(handler, 0x0010, 0x40);
    VL6180_write_byte(handler, 0x0010, 0x00);

}

