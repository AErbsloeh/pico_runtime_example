#include "sens/ad5141_spi.h"


void ad5141_spi_reset_handler_params(ad5141_spi_t *device_config)
{
    device_config->init_done = false;
}


bool ad5141_spi_reset_software(ad5141_spi_t *device_config)
{
    if(!device_config->spi_handler->init_done){
        configure_spi_module(device_config->spi_handler, false, device_config->device_csn);
    } 
    ad5141_spi_reset_handler_params(device_config);

    uint8_t  buffer[2] = {0};
    buffer[0] = 0xB0;
    buffer[1] = 0x00;

    spi_write_blocking(device_config->spi_handler->spi_mod, buffer, 2);
    return true;
}


bool ad5141_spi_control_shutdown(ad5141_spi_t *device_config, bool enable_rdac0, bool enable_rdac1)
{
    if(!device_config->spi_handler->init_done){
        configure_spi_module(device_config->spi_handler, false, device_config->device_csn);
    } 

    // --- Defining buffer values
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

    spi_write_blocking(device_config->spi_handler->spi_mod, buffer, 2);
    return true;
}


bool ad5141_spi_init(ad5141_spi_t *device_config)
{
    if(!device_config->spi_handler->init_done){
        configure_spi_module(device_config->spi_handler, false, device_config->device_csn);
    } 

    ad5141_spi_reset_software(device_config);
    ad5141_spi_control_shutdown(device_config, true, true);    

    device_config->init_done = true;
    return true;
}


bool ad5141_spi_define_level(ad5141_spi_t *device_config, uint8_t rdac_sel, uint8_t pot_position){
    if(device_config->init_done){
        uint8_t  buffer[2] = {0};
        buffer[0] = 0x10 | (rdac_sel & 0x0F);
        buffer[1] = pot_position;
        spi_write_blocking(device_config->spi_handler->spi_mod, buffer, 2);

        return true;
    } else {
        return false;
    } 
}
