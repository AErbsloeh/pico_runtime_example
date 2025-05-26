#include "sens/ltc2668.h"


// ======================================== INTERNAL READ/WRITE COMMANDS ===============================================
int8_t handler_pico_spi_transmission(ltc2668_handler_t *handler, uint8_t command, uint8_t adr, uint16_t data){
    uint8_t buffer_tx[3] = {0x00};
    buffer_tx[0] = ((command & 0x0F) << 0x04) | ((adr & 0x0F) << 0x00);
    buffer_tx[1] = ((data & 0xFF00) >> 0x08);
    buffer_tx[2] = ((data & 0x00FF) >> 0x00);

    return send_data_spi_module(handler->spi_handler, handler->gpio_num_csn, buffer_tx, 3);
}

// ======================================== FUNCTIONS ===============================================
bool ltc2668_init(ltc2668_handler_t *config_device){
    // --- Init of SPI module
    if(!config_device->spi_handler->init_done){
        configure_spi_module(config_device->spi_handler, false);
    }

    bool error_check = true;
    if(config_device->spi_handler->mode != 0){
        printf("... LTC2668 device has the wrong SPI mode - Please check!");
        error_check = false;
    }

    // --- Init of GPIO CS
    gpio_init(config_device->gpio_num_csn);
    gpio_set_dir(config_device->gpio_num_csn, GPIO_OUT);
    gpio_set_drive_strength(config_device->gpio_num_csn, GPIO_DRIVE_STRENGTH_2MA);
    gpio_put(config_device->gpio_num_csn, true);

    // --- Init optional GPIO of device and clear output data
    if(config_device->use_clrn_hw){
        gpio_init(config_device->gpio_num_clrn);
        gpio_set_dir(config_device->gpio_num_clrn, GPIO_OUT);
        gpio_put(config_device->gpio_num_clrn, false);
        gpio_pull_up(config_device->gpio_num_clrn);
        gpio_set_drive_strength(config_device->gpio_num_clrn, GPIO_DRIVE_STRENGTH_2MA);
    }

    // --- Bringing whole device into power down mode
    //handler_pico_spi_transmission(config_device, 0x05, 0x00, 0x0000);
    sleep_ms(10);

    // --- Controlling the reference device
    if(config_device->use_int_vref){
        handler_pico_spi_transmission(config_device, 0x07, 0x00, 0x0000);
    } else {
        handler_pico_spi_transmission(config_device, 0x07, 0x00, 0x0001);
    };
    sleep_ms(10);
    
    // --- Power-Up device
    if(config_device->pwr_up_chnnl == 0xFFFF){
        handler_pico_spi_transmission(config_device, 0x09, 0x00, 0x0000);
        sleep_us(100);
    } else if (config_device->pwr_up_chnnl == 0x0000){
        handler_pico_spi_transmission(config_device, 0x05, 0x00, 0x0000);
        sleep_us(100);
    } else {
        for(uint8_t idx = 0; idx < 16; idx++){
            bool activate_chnnl = (0x0001 << idx) & config_device->pwr_up_chnnl;
            if(activate_chnnl){
                handler_pico_spi_transmission(config_device, 0x01, idx, 0x0000);
                sleep_us(10);
            };
        };
    };
    ltc2668_clear_data(config_device);

    // --- Setting Span Code for all DAC channel
    handler_pico_spi_transmission(config_device, LTC2668_CMD_SPAN_DAC_ALL, 0x00, config_device->vref_range);
    sleep_us(100);    

    // Last step of initialisation
    config_device->init_done = true && error_check;
    return config_device->init_done;
}


void ltc2668_clear_data(ltc2668_handler_t *config_device){
    if(config_device->use_clrn_hw){
        // Do HW reset
        for(uint8_t idx = 0; idx < 4; idx ++){
            gpio_put(config_device->gpio_num_clrn, false);
            sleep_us(5);
            gpio_put(config_device->gpio_num_clrn, true);
            sleep_us(5);
        }
    } else {
        handler_pico_spi_transmission(config_device, LTC2668_CMD_WR_UPD_DAC_ALL, 0x00, 0x8000);
        sleep_us(5);
    }
}


void ltc2668_mux_control(ltc2668_handler_t *config_device, bool enable, uint8_t channel){
    uint16_t mux_data = (enable) ? (0x0010 | channel & 0x000F) : 0x0000;
    handler_pico_spi_transmission(config_device, LTC2668_CMD_MUX_CONTROL, 0x00, mux_data);
}

void ltc2668_write_output_all_channel(ltc2668_handler_t *config_device, uint16_t data){
    handler_pico_spi_transmission(config_device, LTC2668_CMD_WR_DAC_N, 0x00, data);
}


void ltc2668_write_output_single_channel(ltc2668_handler_t *config_device, uint16_t data, uint8_t channel){
    handler_pico_spi_transmission(config_device, LTC2668_CMD_WR_DAC_ALL, channel & 0x0F, data);
}


void ltc2668_update_output_all_channel(ltc2668_handler_t *config_device, uint16_t data){
    uint16_t data_real = (config_device->use_16bit_dev) ? data : (data & 0x0FFF) << 4;
    handler_pico_spi_transmission(config_device, LTC2668_CMD_WR_UPD_DAC_ALL, 0x00, data_real);
}


void ltc2668_update_output_single_channel(ltc2668_handler_t *config_device, uint16_t data, uint8_t channel){
    uint16_t data_real = (config_device->use_16bit_dev) ? data : (data & 0x0FFF) << 4;
    handler_pico_spi_transmission(config_device, LTC2668_CMD_WR_UPD_DAC_N, channel & 0x0F, data_real);
}
