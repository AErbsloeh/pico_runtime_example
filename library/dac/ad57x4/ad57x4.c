#include "dac/ad57x4/ad57x4.h"
#include "hardware/gpio.h"
#include <stdio.h>


// =========================== INTERNAL FUNCTIONS ===========================
int8_t ad57x4_spi_transmission(ad57x4_t *config, bool rnw, uint8_t reg, uint8_t adr, uint16_t data) {
    uint8_t buffer_tx[3];
    buffer_tx[0] = ((rnw) ? 0x80 : 0x00) | ((reg & 0x07) << 3) | ((adr & 0x07) << 0);
    buffer_tx[1] = ((data & 0xFF00) >> 8);
    buffer_tx[2] = ((data & 0x00FF) << 0);

    return send_data_spi_module(config->spi_mod, config->gpio_num_csn, buffer_tx, 3);
}


// =========================== CALLABLE FUNCTIONS ===========================
bool ad57x4_init(ad57x4_t *config){
    if(!config->spi_mod->init_done){
        configure_spi_module(config->spi_mod, false);
    }

    // --- Init of GPIOs
    // Chip Select Line (Active Low)
    gpio_init(config->gpio_num_csn);
    gpio_set_dir(config->gpio_num_csn, GPIO_OUT);
    gpio_put(config->gpio_num_csn, true);
    gpio_set_drive_strength(config->gpio_num_csn, GPIO_DRIVE_STRENGTH_2MA);

    // Data Clear Line (Active Low)
    gpio_init(config->gpio_num_dclr);
    gpio_set_dir(config->gpio_num_dclr, GPIO_OUT);
    gpio_put(config->gpio_num_dclr, true);
    gpio_set_drive_strength(config->gpio_num_dclr, GPIO_DRIVE_STRENGTH_2MA);

    // Load Data Simultanously (Active High: Update on Falling Edge)
    if(config->use_gpio_ldac){
        gpio_init(config->use_gpio_ldac);
        gpio_set_dir(config->use_gpio_ldac, GPIO_OUT);
        gpio_put(config->use_gpio_ldac, false);
        gpio_set_drive_strength(config->use_gpio_ldac, GPIO_DRIVE_STRENGTH_2MA);
    }
    
    // --- Reset of Device
    ad57x4_reset(config);

    // --- Do configuration
    // Writing to the "Power control register"
    uint16_t data = 0x0000 | config->en_pwr_chnnl;
    printf("Data Power Control: %d\n", data);
    ad57x4_spi_transmission(config, false, AD57x4_REG_PWR, 0x00, data);
    
    // Writing to the "Control register"
    data = ((config->spi_sdo_disable) ? 0x0001 : 0x0000);
    ad57x4_spi_transmission(config, false, AD57x4_REG_CNTRL, 0x01, data);

    // Writing to the "Output range select register"
    data = 0x0000 | config->range_mode;
    ad57x4_spi_transmission(config, false, AD57x4_REG_RANGE, AD57x4_ADR_DAC_ALL, data);

    config->init_done = true;
    return config->init_done;
}


bool ad57x4_reset(ad57x4_t *config){
    if(config->use_gpio_dclr){
        for (uint8_t idx = 0; idx < 3; idx++) {
            gpio_put(config->gpio_num_dclr, false);
            sleep_us(10);
            gpio_put(config->gpio_num_dclr, true);
            sleep_us(10);
        }
    } else {
        ad57x4_spi_transmission(config, false, AD57x4_REG_CNTRL, 0x04, 0x0000);
    };    
}


int8_t ad57x4_update_data(ad57x4_t *config, uint8_t chnnl, uint16_t data){
    uint16_t data0 = 0;
    switch(config->bitwidth){
        case 16:    data0 = ((data & 0xFFFF) << 0);  break;
        case 14:    data0 = ((data & 0x3FFF) << 2);  break;
        case 12:    data0 = ((data & 0x0FFF) << 4);  break;
        default:    data0 = ((data & 0xFFFF) << 0);  break;
    }
    return ad57x4_spi_transmission(config, false, AD57x4_REG_DATA, chnnl, data0);
}
