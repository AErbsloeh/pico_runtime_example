#include "sens/adgs1208.h"


// ======================================== FUNCTIONS ===============================================
bool adgs1208_init(adgs1208_t *config_device){
    if(!config_device->spi_handler->init_done){
        configure_spi_module(config_device->spi_handler, false);
    }

    // --- Init of GPIO CS
    gpio_init(config_device->gpio_num_csn);
    gpio_set_dir(config_device->gpio_num_csn, GPIO_OUT);
    gpio_set_drive_strength(config_device->gpio_num_csn, GPIO_DRIVE_STRENGTH_2MA);
    gpio_put(config_device->gpio_num_csn, true);

    if(config_device->use_rstn_hw){
        gpio_init(config_device->gpio_num_rstn);
        gpio_set_dir(config_device->gpio_num_rstn, GPIO_OUT);
        gpio_put(config_device->gpio_num_rstn, false);
    };
    adgs1208_reset(config_device);
    adgs1208_enable_daisy_chain(config_device);    

    config_device->init_done = true;
    return true;
}


void adgs1208_enable_daisy_chain(adgs1208_t *config_device){
    // Enable Daisy Chain
    uint8_t buffer[2] = {0x00};
    for(uint8_t idx = 0; idx < config_device->num_device_dc; idx++){
        gpio_put(config_device->gpio_num_csn, false);
        buffer[0] = 0x25;
        buffer[1] = 0x00; 
        spi_write_blocking(config_device->spi_handler->spi_mod, buffer, 2);
        gpio_put(config_device->gpio_num_csn, true); 
        sleep_us(500);
    }
}


void adgs1208_reset(adgs1208_t *config_device){
    // Init phase
    if(!config_device->spi_handler->init_done){
        configure_spi_module(config_device->spi_handler, config_device->gpio_num_csn, false);
    }
    
    if(config_device->use_rstn_hw){
        // --- Hardware Reset
        for(uint8_t repeat=0; repeat < 2; repeat++){
            gpio_put(config_device->gpio_num_rstn, false);
            sleep_ms(10);
            gpio_put(config_device->gpio_num_rstn, true);
            sleep_ms(10);
        };        
    } else {
        // --- Software Reset
        uint8_t buffer[2] = {0};   
       
        // Step #1        
        for(uint8_t idx = 0; idx < config_device->num_device_dc; idx++){
            gpio_put(config_device->gpio_num_csn, false);
            buffer[0] = 0x0B;
            buffer[1] = 0xA3; 
            spi_write_blocking(config_device->spi_handler->spi_mod, buffer, 2);
            gpio_put(config_device->gpio_num_csn, true);   
            sleep_us(200);
        }
        // Step #2
        for(uint8_t idx = 0; idx < config_device->num_device_dc; idx++){
            gpio_put(config_device->gpio_num_csn, false);
            buffer[0] = 0x0B;
            buffer[1] = 0x05; 
            spi_write_blocking(config_device->spi_handler->spi_mod, buffer, 2);
            gpio_put(config_device->gpio_num_csn, true);
            sleep_us(200);
        }            
    };
    sleep_us(500);
}


uint8_t adgs1208_get_data(adgs1208_t *config_device, uint8_t mux_sel){
    uint8_t gpio4 = (config_device->gpio_4_state) ? 0x80 : 0x00;
    uint8_t gpio3 = (config_device->gpio_3_state) ? 0x40 : 0x00;
    uint8_t gpio2 = (config_device->gpio_2_state) ? 0x20 : 0x00;
    uint8_t gpio1 = (config_device->gpio_1_state) ? 0x10 : 0x00;

    uint8_t data = 0x00;
    switch(mux_sel){
        case 1:     data = 0x01; break;
        case 2:     data = 0x03; break;
        case 3:     data = 0x05; break;
        case 4:     data = 0x07; break;
        case 5:     data = 0x09; break;
        case 6:     data = 0x0B; break;
        case 7:     data = 0x0C; break;
        case 8:     data = 0x0F; break;
        default:    data = 0x00; break;
    };
    return gpio4 | gpio3 | gpio2 | gpio1 | data;
}


bool adgs1208_define_output(adgs1208_t *config_device, const uint8_t *mux_data){
    if(config_device->num_device_dc == 1){
        // Single Trial
        uint8_t buffer[2] = {0x00};

        gpio_put(config_device->gpio_num_csn, false);
        buffer[0] = 0x01;
        buffer[1] = adgs1208_get_data(config_device, mux_data[0]);
        spi_write_blocking(config_device->spi_handler->spi_mod, buffer, 2);
        gpio_put(config_device->gpio_num_csn, true);
    } else {
        // Daisy Chain Mode
        uint8_t buffer[1] = {0x00};
        
        gpio_put(config_device->gpio_num_csn, false);
        for(uint8_t idx = 0; idx < config_device->num_device_dc; idx++){
            buffer[0] = adgs1208_get_data(config_device, mux_data[idx]);
            spi_write_blocking(config_device->spi_handler->spi_mod, buffer, 1);
        };
        gpio_put(config_device->gpio_num_csn, true);
        printf("\n");
    }
}
