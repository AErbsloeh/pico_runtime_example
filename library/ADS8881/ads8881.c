#include "sens/ads8881.h"


// ================================= FUNCTIONS =================================
bool ads8881_init(ads8881_handler_t *handler){
    if(!handler->spi_handler->init_done){
        configure_spi_module(handler->spi_handler, handler->gpio_num_csn, false);
    };

    if(!handler->spi_handler->mode == 0){
        printf("ADC SPI Mode is not 0");
        handler->init_done = false;
    } else {
        // --- Init of GPIO CS
        gpio_init(handler->gpio_num_csn);
        gpio_set_dir(handler->gpio_num_csn, GPIO_OUT);
        gpio_set_drive_strength(handler->gpio_num_csn, GPIO_DRIVE_STRENGTH_2MA);
        gpio_put(handler->gpio_num_csn, true);

        // --- Inif of CONVST pin
        if(handler->gpio_num_convst){
            gpio_init(handler->gpio_num_convst);
            gpio_set_dir(handler->gpio_num_convst, GPIO_OUT);
            gpio_put(handler->gpio_num_convst, false);
        };
        handler->init_done = true;
    }    
    return handler->init_done;
}


uint32_t ads8881_rqst_data_cheesy(ads8881_handler_t *handler){
    uint8_t buffer_rx[3] = {0x00};

    // Start Data Acquisition
    gpio_put(handler->gpio_num_csn, true);
    spi_read_blocking(handler->spi_handler->spi_mod, 0xFF, buffer_rx, 1);
    
    // Get Data
    spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, 3);
    gpio_put(handler->gpio_num_csn, false);

    return translate_array_into_uint32(buffer_rx, 3);
}


uint32_t ads8881_rqst_data_normal(ads8881_handler_t *handler){
    uint8_t buffer_rx[3] = {0x00};

    // Start Data Acquisition
    gpio_put(handler->gpio_num_csn, true);
    gpio_put(handler->gpio_num_convst, true);
    spi_read_blocking(handler->spi_handler->spi_mod, 0xFF, buffer_rx, 1);
    gpio_put(handler->gpio_num_csn, false);

    // Get Data
    spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, 2);
    gpio_put(handler->gpio_num_csn, true);
    gpio_put(handler->gpio_num_convst, false);

    return translate_array_into_uint32(buffer_rx, 3);
}


uint32_t ads8881_rqst_data(ads8881_handler_t *handler){
    if(!handler->init_done){
        ads8881_init(handler);
    } else {
        return (handler->gpio_num_convst) ? ads8881_rqst_data_normal(handler) : ads8881_rqst_data_cheesy(handler);
    }
}
