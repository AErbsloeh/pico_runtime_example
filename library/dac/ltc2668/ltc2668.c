#include "dac/ltc2668/ltc2668.h"
#include "hardware/gpio.h"


// ======================================== DEFINITIONS ===============================================
#define CMD_WR_CODE_DAC_N       0x00
#define CMD_WR_CODE_DAC_ALL     0x08
#define CMD_WR_SPAN_DAC_N       0x06
#define CMD_WR_SPAN_DAC_ALL     0x0E
#define CMD_UPD_DAC_N           0x01
#define CMD_UPD_DAC_ALL         0x09
#define CMD_WR_UPD_DAC_N        0x03
#define CMD_WR_UPD_DAC_ALL      0x02
#define CMD_PWR_DWN_DAC_N       0x04
#define CMD_PWR_DWN_ALL         0x05
#define CMD_MUX_CONTROL         0x0B
#define CMD_CONFIG              0x07


// ======================================== INTERNAL READ/WRITE COMMANDS ===============================================
int8_t handler_pico_spi_transmission(ltc2668_t *cnf, uint8_t cmd, uint8_t adr, uint16_t data){
    uint8_t buffer_tx[3] = {0x00};
    buffer_tx[0] = ((cmd & 0x0F) << 0x04) | ((adr & 0x0F) << 0x00);
    buffer_tx[1] = ((data & 0xFF00) >> 0x08);
    buffer_tx[2] = ((data & 0x00FF) >> 0x00);

    return send_data_spi_module(cnf->spi_handler, cnf->gpio_num_csn, buffer_tx, 3);
}

// ======================================== FUNCTIONS ===============================================
bool ltc2668_init(ltc2668_t *cnf){
    // --- Init of SPI module
    if(!cnf->spi_handler->init_done){
        configure_spi_module(cnf->spi_handler, false);
    }

    bool error_check = true;
    if(cnf->spi_handler->mode != 0){
        //printf("... LTC2668 device has the wrong SPI mode - Please check!");
        error_check = false;
    }

    // --- Init of GPIO CS
    gpio_init(cnf->gpio_num_csn);
    gpio_set_dir(cnf->gpio_num_csn, GPIO_OUT);
    gpio_set_drive_strength(cnf->gpio_num_csn, GPIO_DRIVE_STRENGTH_2MA);
    gpio_put(cnf->gpio_num_csn, true);

    // --- Init optional GPIO of device and clear output data
    if(cnf->use_clrn_hw){
        gpio_init(cnf->gpio_num_clrn);
        gpio_set_dir(cnf->gpio_num_clrn, GPIO_OUT);
        gpio_put(cnf->gpio_num_clrn, false);
        gpio_pull_up(cnf->gpio_num_clrn);
        gpio_set_drive_strength(cnf->gpio_num_clrn, GPIO_DRIVE_STRENGTH_2MA);
    }

    // --- Bringing whole device into power down mode
    //handler_pico_spi_transmission(cnf, CMD_PWR_DWN_ALL, 0x00, 0x0000);
    sleep_ms(10);

    // --- Controlling the reference device
    if(cnf->use_int_vref){
        handler_pico_spi_transmission(cnf, CMD_CONFIG, 0x00, 0x0000);
    } else {
        handler_pico_spi_transmission(cnf, CMD_CONFIG, 0x00, 0x0001);
    };
    sleep_ms(10);
    
    // --- Power-Up device
    if(cnf->pwr_up_chnnl == 0xFFFF){
        handler_pico_spi_transmission(cnf, CMD_UPD_DAC_ALL, 0x00, 0x0000);
        sleep_us(100);
    } else if (cnf->pwr_up_chnnl == 0x0000){
        handler_pico_spi_transmission(cnf, CMD_PWR_DWN_ALL, 0x00, 0x0000);
        sleep_us(100);
    } else {
        for(uint8_t idx = 0; idx < 16; idx++){
            bool activate_chnnl = (0x0001 << idx) & cnf->pwr_up_chnnl;
            if(activate_chnnl){
                handler_pico_spi_transmission(cnf, CMD_UPD_DAC_N, idx, 0x0000);
                sleep_us(10);
            };
        };
    };    
    // --- Setting Span Code for all DAC channel
    for(uint8_t chnl=0; chnl < 16; chnl++){
        handler_pico_spi_transmission(cnf, CMD_WR_SPAN_DAC_N, chnl, cnf->vref_range & 0x0007);
        sleep_us(10);
    }  

    ltc2668_clear_data_soft(cnf);
    sleep_us(100);  

    // Last step of initialisation
    cnf->init_done = true && error_check;
    return cnf->init_done;
}


void ltc2668_pwr_dwn_device(ltc2668_t *cnf){
	handler_pico_spi_transmission(cnf, CMD_PWR_DWN_ALL, 0x00, 0x0000);
}


void ltc2668_update_vrange(ltc2668_t *cnf, uint8_t chnl, uint8_t vref_mode){
    handler_pico_spi_transmission(cnf, CMD_WR_SPAN_DAC_N, chnl, vref_mode);
    sleep_us(10);
}


void ltc2668_clear_data_soft(ltc2668_t *cnf){
    for(uint8_t chnl=0; chnl < 16; chnl++){
        handler_pico_spi_transmission(cnf, CMD_WR_CODE_DAC_N, chnl, 0x8000);
        sleep_us(5);
    };        
}


void ltc2668_clear_data(ltc2668_t *cnf){
    if(cnf->use_clrn_hw){
        // Do HW reset
        for(uint8_t idx = 0; idx < 4; idx ++){
            gpio_put(cnf->gpio_num_clrn, false);
            sleep_us(5);
            gpio_put(cnf->gpio_num_clrn, true);
            sleep_us(5);
        }
    } else {
        // Do SW reset
        ltc2668_clear_data_soft(cnf);    
    }
}


void ltc2668_mux_control(ltc2668_t *cnf, bool enable, uint8_t chnnl){
    uint16_t mux_data = (enable) ? (0x0010 | chnnl & 0x000F) : 0x0000;
    handler_pico_spi_transmission(cnf, CMD_MUX_CONTROL, 0x00, mux_data);
}


void ltc2668_write_output_all_channel(ltc2668_t *cnf, uint16_t data){
    handler_pico_spi_transmission(cnf, CMD_WR_CODE_DAC_ALL, 0x00, data);
}


void ltc2668_write_output_single_channel(ltc2668_t *cnf, uint16_t data, uint8_t chnnl){
    handler_pico_spi_transmission(cnf, CMD_WR_CODE_DAC_N, chnnl & 0x0F, data);
}


void ltc2668_update_output_all_channel(ltc2668_t *cnf, uint16_t data){
    uint16_t data_real = (cnf->use_16bit_dev) ? data : (data & 0x0FFF) << 4;
    handler_pico_spi_transmission(cnf, CMD_WR_UPD_DAC_ALL, 0x00, data_real);
}


void ltc2668_update_output_single_channel(ltc2668_t *cnf, uint16_t data, uint8_t chnnl){
    uint16_t data_real = (cnf->use_16bit_dev) ? data : (data & 0x0FFF) << 4;
    handler_pico_spi_transmission(cnf, CMD_WR_UPD_DAC_N, chnnl & 0x0F, data_real);
}
