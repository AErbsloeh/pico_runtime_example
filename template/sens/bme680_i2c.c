#include "sens/bme680_i2c.h"


// ======================================== FUNCTIONS ===============================================
bool BME680_i2c_read_id(bme680_i2c_t *handler, bool print_id){
    uint8_t buffer_tx[1] = {0xD0};
    uint8_t buffer_rx[1] = {0x00};
    construct_i2c_read_data(handler->i2c_mod, BME680_I2C_ADR + handler->state_sdo_vddio, buffer_tx, 1, buffer_tx, 1);
    if(print_id){
        printf("Get ID from BME680: %x\n", buffer_rx[0]);
    }
    return (buffer_rx[0] == BME680_I2C_ADR + handler->state_sdo_vddio);
}


bool BME680_i2c_soft_reset(bme680_i2c_t *handler){
    uint8_t buffer_tx[2] = {0xE0};
    buffer_tx[1] = 0xB6;
    construct_i2c_write_data(handler->i2c_mod, BME680_I2C_ADR + handler->state_sdo_vddio, buffer_tx, 2);
    return true;
}
