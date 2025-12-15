#include "poti/ad5142a/ad5142a.h"
#include "hardware/gpio.h"


uint8_t ad5142a_get_device_adr(uint8_t mode_adr)
{
    uint8_t adr = 0x00;
    switch (mode_adr){
        case 0:     //ADDR0, ADDR1 = V_L, V_L 
            adr = 0x20;
            break;
        case 1:     //ADDR0, ADDR1 = NC, V_L 
            adr = 0x22;
            break;
        case 2:     //ADDR0, ADDR1 = GND, V_L 
            adr = 0x23;
            break;
        case 3:     //ADDR0, ADDR1 = V_L, NC 
            adr = 0x28;
            break;
        case 4:     //ADDR0, ADDR1 = NC, NC 
            adr = 0x2A;
            break;
        case 5:     //ADDR0, ADDR1 = GND, NC 
            adr = 0x2B;
            break;
        case 6:     //ADDR0, ADDR1 = V_L, GND 
            adr = 0x2C;
            break;
        case 7:     //ADDR0, ADDR1 = NC, GND 
            adr = 0x2E;
            break;
        case 8:     //ADDR0, ADDR1 = GND, GND
            adr = 0x2F;
            break;
        default:
            adr = 0x00;
            break;
    }
    return adr;
}

void ad5142a_reset_handler_params(ad5142a_t *device_config)
{
    device_config->init_done = false;
}


bool ad5142a_reset_software(ad5142a_t *device_config)
{
    ad5142a_reset_handler_params(device_config);

    uint8_t  buffer[2] = {0};
    buffer[0] = 0xB0;
    buffer[1] = 0x00;

    i2c_write_blocking(device_config->i2c_handler->i2c_mod, device_config->adr, buffer, 2, false);
    return true;
}


bool ad5142a_control_shutdown(ad5142a_t *device_config, bool enable_rdac0, bool enable_rdac1)
{
    uint8_t  buffer[2] = {0};
    if (enable_rdac0 && !enable_rdac1){
        buffer[0] = 0xC0;
        buffer[1] = (enable_rdac0) ? 0x00 : 0x01;
    } else if (!enable_rdac0 && enable_rdac1){
        buffer[0] = 0xC1;
        buffer[1] = (enable_rdac1) ? 0x00 : 0x01;
    } else {
        buffer[0] = 0xC8;
        buffer[1] = (enable_rdac0 && enable_rdac1) ? 0x00 : 0x01;
    }

    i2c_write_blocking(device_config->i2c_handler->i2c_mod, device_config->adr, buffer, 2, false);
    return true;
}


bool ad5142a_init(ad5142a_t *device_config, uint8_t mode_adr)
{
    init_i2c_module(device_config->i2c_handler);
    device_config->adr = ad5142a_get_device_adr(mode_adr);

    if(check_i2c_bus_for_device_specific(device_config->i2c_handler, device_config->adr)){
        ad5142a_reset_software(device_config);
        ad5142a_control_shutdown(device_config, true, true);    

        device_config->init_done = true;
    } else {
        device_config->init_done = false;
    }
    return device_config->init_done;
}


bool ad5142a_define_level(ad5142a_t *device_config, uint8_t rdac_sel, uint8_t pot_position){
    if(device_config->init_done){
        uint8_t  buffer[2] = {0};
        buffer[0] = 0x10 | (rdac_sel & 0x0F);
        buffer[1] = pot_position;
        i2c_write_blocking(device_config->i2c_handler->i2c_mod, device_config->adr, buffer, 2, false);

        return true;
    } else {
        return false;
    } 
}
