#include "sens/ad526x.h"


bool AD5260_write_data(ad526x_device_t *handler, uint8_t data){
    uint8_t buffer_tx[1] = {data};
    return send_data_spi_module(handler->spi_handler, handler->gpio_cs, buffer_tx, 1);    
}

bool AD5262_write_data(ad526x_device_t *handler, bool chnnl, uint8_t data){
    uint8_t buffer_tx[2] = {0x00};
    buffer_tx[0] = (chnnl) ? 0x01 : 0x00;
    buffer_tx[1] = data;
    return send_data_spi_module(handler->spi_handler, handler->gpio_cs, buffer_tx, 2);    
}

bool AD526x_write_data(ad526x_device_t *handler, bool chnnl, uint8_t data){
    return (handler->device_mode) ? AD5262_write_data(handler, chnnl, data) : AD5260_write_data(handler, data);
}


// ======================================== FUNCTIONS ===============================================
bool ad526x_init(ad526x_device_t *handler){
    if(!handler->spi_handler->init_done){
        configure_spi_module(handler->spi_handler);
    }

    ad526x_soft_reset(handler);
    ad526x_define_shutdown(handler);
    return true;
}


bool ad526x_soft_reset(ad526x_device_t *handler){
    for(uint8_t ite=0; ite < 4; ite++){
        AD526x_write_data(handler, false, 0x00);
        sleep_us(100);
        AD526x_write_data(handler, true, 0x00);
        sleep_us(100);
    };
    return true;
}


bool ad526x_define_shutdown(ad526x_device_t *handler){
    uint8_t cmd = (handler->shutdown) ? 0x00 : 0x20;
    return AD526x_write_data(handler, cmd, 0x00);
}


bool ad526x_define_output(ad526x_device_t *handler, bool chnnl, uint8_t position){
    return AD526x_write_data(handler, chnnl, position);
}

