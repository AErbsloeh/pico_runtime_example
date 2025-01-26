#include "sens/m24m01e.h"


bool M24M01E_write_byte(m24m01e_handler_t *handler, uint16_t command, uint8_t data){
    uint8_t buffer_tx[2] = {0};
    buffer_tx[0] = (command & 0xFF00) >> 8;
    buffer_tx[1] = (command & 0x00FF) >> 0;

    i2c_write_blocking(handler->i2c_mod->i2c_mod, M24M01E_I2C_ADR, buffer_tx, 2, false);
    sleep_us(50);
    return true;
}


uint64_t M24M01E_read_data(m24m01e_handler_t *handler, uint16_t command, uint8_t buffer_rx[], uint8_t size){
    uint8_t buffer_tx[2] = {0};
    buffer_tx[0] = (command & 0xFF00) >> 8;
    buffer_tx[1] = (command & 0x00FF) >> 0;

    i2c_write_blocking(handler->i2c_mod->i2c_mod, M24M01E_I2C_ADR, buffer_tx, 2, true);
    sleep_us(10);
    while (i2c_read_blocking(handler->i2c_mod->i2c_mod, M24M01E_I2C_ADR, buffer_rx, size, false) == PICO_ERROR_GENERIC){
        sleep_ms(2);
    }; 

    uint64_t raw_data = 0;
    for(uint8_t idx = 0; idx < size; idx++){
        raw_data |= buffer_rx[idx] << 8*idx;
    }
    return raw_data;
}


bool M24M01E_read_id(m24m01e_handler_t *handler, bool print_id){
    uint8_t buffer[1] = {0x00};
    uint8_t id = M24M01E_read_data(handler, 0x0000, buffer, 1);
    if(print_id){
        printf("Get ID of M24M01E: %x\n", id);
    }
    return id && 0xB4;
}


bool M24M01E_init(m24m01e_handler_t *handler){
    gpio_init(handler->gpio_rnw);
    gpio_set_dir(handler->gpio_rnw, true);
    gpio_put(handler->gpio_rnw, true);
}
