#include "dac/ad5765/ad5765.h"
#include "hardware/gpio.h"
#include <stdio.h>


// ======================================== INTERNAL READ/WRITE COMMANDS ===============================================
int8_t ad5765_spi_transmission(ad5765_t *handler, bool rnw, uint8_t cmd, uint8_t adr, uint16_t data){
    uint8_t buffer_tx[3] = {0x00};
    buffer_tx[0] = ((rnw) ? 0x80 : 0x00) | ((cmd & 0x07) << 3) | ((adr & 0x07) << 0);
    buffer_tx[1] = (data & 0xFF00) >> 8;
    buffer_tx[2] = (data & 0x00FF) >> 0;

    return send_data_spi_module(handler->spi_handler, handler->gpio_num_csn, buffer_tx, 3);
};


// ======================================== FUNCTIONS ===============================================
bool ad5765_init(ad5765_t *handler){
    if(!handler->spi_handler->init_done){
        gpio_init(handler->gpio_num_csn);
        gpio_set_dir(handler->gpio_num_csn, GPIO_OUT);
        gpio_put(handler->gpio_num_csn, true);

        configure_spi_module(handler->spi_handler, false);
    };

    bool state_okay = true;
    if(handler->spi_handler->fspi_khz > 30000){
        printf("... SPI clock higher than maximal allowed 30 MHz. Please change!");
        state_okay = false;
    };

    if(handler->spi_handler->mode != 1){
        printf("... please change SPI mode to 1");
        state_okay = false;
    }

    // --- Init of GPIOs
    // Chip Select Line (Active Low)
    gpio_init(handler->gpio_num_csn);
    gpio_set_dir(handler->gpio_num_csn, GPIO_OUT);
    gpio_set_drive_strength(handler->gpio_num_csn, GPIO_DRIVE_STRENGTH_2MA);
    gpio_put(handler->gpio_num_csn, true);

    // Reset DAC Data Line (Active Low)
    if(handler->use_gpio_rst){
        gpio_init(handler->gpio_num_rst);
        gpio_set_dir(handler->gpio_num_rst, GPIO_OUT);
        gpio_set_drive_strength(handler->gpio_num_rst, GPIO_DRIVE_STRENGTH_2MA);
        gpio_put(handler->gpio_num_rst, false);
    };

    // Load and Update DAC Data (Active Low)
    if(handler->use_gpio_ldac){
        gpio_init(handler->gpio_num_ldac);
        gpio_set_dir(handler->gpio_num_ldac, GPIO_OUT);
        gpio_set_drive_strength(handler->gpio_num_ldac, GPIO_DRIVE_STRENGTH_2MA);
        gpio_put(handler->gpio_num_ldac, false);
    };

    // Clear DAC Data (Active Low)
    if(handler->use_gpio_clr){
        gpio_init(handler->gpio_num_clr);
        gpio_set_dir(handler->gpio_num_clr, GPIO_OUT);
        gpio_set_drive_strength(handler->gpio_num_clr, GPIO_DRIVE_STRENGTH_2MA);
        gpio_put(handler->gpio_num_clr, false);
    };

    // --- Reset Device
    ad5765_reset(handler);
    ad5765_clear_data(handler);
    
    // --- Init of Device
    uint16_t data = 0x0015 | ((handler->state_gpio1) ? 0x0008 : 0x0000) | ((handler->state_gpio0) ? 0x0002 : 0x0000);
    ad5765_spi_transmission(handler, false, AD5765_REG_FUNC, 0x01, data);
    ad5765_spi_transmission(handler, false, AD5765_REG_GAIN_COARSE, AD5765_ADR_DAC_ALL, 0x00);
    ad5765_spi_transmission(handler, false, AD5765_REG_GAIN_FINE, AD5765_ADR_DAC_ALL, 0x00);
    ad5765_spi_transmission(handler, false, AD5765_REG_OFFSET, AD5765_ADR_DAC_ALL, 0x00);

    handler->init_done = true && state_okay;
    return handler->init_done;
};


bool ad5765_reset(ad5765_t *handler){
    if(handler->use_gpio_rst) {
        for(uint8_t idx=0; idx < 2; idx++){
            gpio_put(handler->gpio_num_rst, false);
            sleep_us(20);
            gpio_put(handler->gpio_num_rst, true);
            sleep_us(20);
        };
    } else {
        ad5765_spi_transmission(handler, false, AD5765_REG_FUNC, 0x04, 0x0000);
    };
    return true;
};


int8_t ad5765_update_data(ad5765_t *handler, bool update_data, uint8_t chnnl, uint16_t data){
    int8_t num = 0;
    if(update_data){
        if(!handler->use_gpio_ldac){
            num = ad5765_spi_transmission(handler, false, AD5765_REG_DATA, chnnl, data);
            ad5765_spi_transmission(handler, false, AD5765_REG_FUNC, 0x05, 0x0000);
        } else {
            gpio_put(handler->gpio_num_ldac, true);
            num = ad5765_spi_transmission(handler, false, AD5765_REG_DATA, chnnl, data);
            sleep_us(1);
            gpio_put(handler->gpio_num_ldac, false);
        };
    } else {
        if(handler->use_gpio_ldac){
            gpio_put(handler->gpio_num_ldac, true);
        }
        num = ad5765_spi_transmission(handler, false, AD5765_REG_DATA, chnnl, data);
    };
    return num;
};


int8_t ad5765_clear_data(ad5765_t *handler){
    int8_t num = 0;
    if(!handler->use_gpio_clr){
        num = ad5765_spi_transmission(handler, false, AD5765_REG_FUNC, 0x04, 0x0000);
    } else {
        gpio_put(handler->gpio_num_clr, false);
        sleep_us(100);
        gpio_put(handler->gpio_num_clr, true);
    };
    return num;
};


int8_t ad5765_update_gpio(ad5765_t *handler, bool sel_gpio, bool state_gpio){
    if(sel_gpio){
        handler->state_gpio1 = state_gpio;
    } else {
        handler->state_gpio0 = state_gpio;
    };
    uint16_t data = 0x0015 | ((handler->state_gpio1) ? 0x0008 : 0x0000) | ((handler->state_gpio0) ? 0x0002 : 0x0000);
    return ad5765_spi_transmission(handler, false, AD5765_REG_FUNC, 0x01, data);
};
