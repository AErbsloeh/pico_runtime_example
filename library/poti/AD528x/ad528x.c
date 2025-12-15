#include "poti/ad528x/ad528x.h"
#include "hardware/gpio.h"


// ======================================== INTERNAL READ/WRITE COMMANDS ===============================================
uint8_t AD528x_get_adress(ad528x_t *handler){
    uint8_t right_adr = AD528X_BASIC_I2C_ADR;
    switch(handler->mode_sel){
        case 0:     right_adr |= 0x00;  break;
        case 1:     right_adr |= 0x01;  break;
        case 2:     right_adr |= 0x02;  break;
        case 3:     right_adr |= 0x03;  break;        
        default:    right_adr |= 0x00;  break;
    };
    return right_adr;
}


uint8_t AD528x_create_command(ad528x_t *handler){
    return ((handler->shutdown) ? 0x00 : 0x20) | ((handler->gpio_state && AD528X_GPIO_PIN0) << 0x10) | ((handler->gpio_state && AD528X_GPIO_PIN1) << 0x08);
}


bool AD528x_write_data(ad528x_t *handler, uint8_t command, uint8_t data){
    uint8_t buffer_tx[2] = {0x00};
    buffer_tx[0] = 0x00;
    buffer_tx[1] = 0x00;

    return construct_i2c_write_data(handler->i2c_handler, AD528x_get_adress(handler), buffer_tx, 2);    
}


// ======================================== FUNCTIONS ===============================================
bool ad528x_init(ad528x_t *handler){
    init_i2c_module(handler->i2c_handler);

    if(check_i2c_bus_for_device_specific(handler->i2c_handler, AD528x_get_adress(handler))){
        ad528x_soft_reset(handler);
        ad528x_define_shutdown(handler);
        handler->init_done = true;
    } else {
        handler->init_done = false;
    }    
    return handler->init_done;
}


bool ad528x_soft_reset(ad528x_t *handler){
    for(uint8_t ite=0; ite < 4; ite++){
        AD528x_write_data(handler, 0x40, 0x00);
        sleep_us(100);
        AD528x_write_data(handler, 0x00, 0x00);
        sleep_us(100);
    };
    return true;
}


bool ad528x_define_shutdown(ad528x_t *handler){
    uint8_t cmd = (handler->shutdown) ? 0x00 : 0x20;
    return AD528x_write_data(handler, cmd, 0x00);
}


bool ad528x_define_gpio_output(ad528x_t *handler){
    return AD528x_write_data(handler, AD528x_create_command(handler), 0x00);
}


bool ad528x_define_output(ad528x_t *handler, bool chnnl_b, uint8_t position){
    uint8_t cmd = AD528x_create_command(handler) | ((chnnl_b) ? 0x80 : 0x00);
    return AD528x_write_data(handler, cmd, position);
}

