#include "sens/bme680_i2c.h"


bool BME680_i2c_write_byte(bme680_i2c_handler_t *handler, uint8_t command, uint8_t data){
    uint8_t buffer_tx[2] = {command};
    buffer_tx[1] = data;

    i2c_write_blocking(handler->i2c_mod->i2c_mod, BME680_I2C_ADR + handler->state_sdo_vddio, buffer_tx, sizeof(buffer_tx), false);
    sleep_us(50);
    return true;
}


bool BME680_i2c_write_block(bme680_i2c_handler_t *handler, uint8_t data[], uint8_t size){
    i2c_write_blocking(handler->i2c_mod->i2c_mod, BME680_I2C_ADR + handler->state_sdo_vddio, data, size, false);
    return true;
}


uint64_t BME680_i2c_read_data(bme680_i2c_handler_t *handler, uint8_t command, uint8_t buffer_rx[], uint8_t size){
    uint8_t buffer_tx[1] = {command};

    i2c_write_blocking(handler->i2c_mod->i2c_mod, BME680_I2C_ADR + handler->state_sdo_vddio, buffer_tx, sizeof(buffer_tx), true);
    sleep_us(10);
    while (i2c_read_blocking(handler->i2c_mod->i2c_mod, BME680_I2C_ADR + handler->state_sdo_vddio, buffer_rx, size, false) == PICO_ERROR_GENERIC){
        sleep_ms(2);
    }; 

    uint64_t raw_data = 0;
    for(uint8_t idx = 0; idx < size; idx++){
        raw_data |= buffer_rx[idx] << 8*idx;
    }
    return raw_data;
}


bool BME680_i2c_read_id(bme680_i2c_handler_t *handler, bool print_id){
    uint8_t buffer[1] = {0};
    BME680_i2c_read_data(handler, 0xD0, buffer, 1);
    if(print_id){
        printf("Get ID from BME680: %x", buffer[0]);
    }
    return buffer[0] && 0x61;
}


bool BME680_i2c_soft_reset(bme680_i2c_handler_t *handler){
    BME680_i2c_write_byte(handler, 0xE0, 0xB6);
    return true;
}
