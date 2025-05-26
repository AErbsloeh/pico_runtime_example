#include "sens/ads8881.h"


// ================================= FUNCTIONS =================================
bool ads8881_init(ads8881_handler_t *handler){
    if(!handler->spi_handler->init_done){
        configure_spi_module(handler->spi_handler, false);
    };

    if(!handler->spi_handler->mode == 0){
        printf("ADC SPI Mode is not 0");
        handler->init_done = false;
    } else {
        // --- Inif of CONVST pin
        gpio_init(handler->gpio_num_conv);
        gpio_set_dir(handler->gpio_num_conv, GPIO_OUT);
        gpio_put(handler->gpio_num_conv, false);
        handler->init_done = true;

        if(handler->adc_mode > 1){
            // --- Init of CS pin
            gpio_init(handler->gpio_num_csn);
            gpio_set_dir(handler->gpio_num_csn, GPIO_OUT);
            gpio_set_drive_strength(handler->gpio_num_csn, GPIO_DRIVE_STRENGTH_2MA);
            gpio_put(handler->gpio_num_csn, true);
        }
    }    
    return handler->init_done;
}

// =========================================== FUNCTIONS FOR NORMAL MODE FROM DATASHEET ==========================================
int ads8881_three_wire_wo_busy(ads8881_handler_t *handler, uint8_t *buffer_rx, uint8_t len){
    // --- DIN should be tied to VDDD and DOUT is used without pullup (normal DOUT is tri-state)
    // Conversion Phase
    gpio_put(handler->gpio_num_conv, true);
    sleep_us(1);   
    gpio_put(handler->gpio_num_conv, false);

    // Acquisition Phase
    return spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, len);
}


int ads8881_three_wire_with_busy(ads8881_handler_t *handler, uint8_t *buffer_rx, uint8_t len){
    // --- DIN should be tied to VDDD and DOUT is used with pullup
    // Conversion Phase
    gpio_put(handler->gpio_num_conv, true);
    for(uint16_t i = 0; i < 999; i++);
    gpio_put(handler->gpio_num_conv, false);

    // Acquisition Phase
    while(gpio_get(handler->spi_handler->pin_miso));  
    return spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, len);
}


int ads8881_four_wire_wo_busy(ads8881_handler_t *handler, uint8_t *buffer_rx, uint8_t len){
    // --- CS goes to DIN and DOUT is used without pullup (normal DOUT is tri-state)
    // Conversion Phase
    gpio_put(handler->gpio_num_conv, true);
    sleep_us(1);   
    gpio_put(handler->gpio_num_csn, false);
    
    // Acquisition Phase
    gpio_put(handler->gpio_num_csn, false);
    int num_bytes = spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, len);
    gpio_put(handler->gpio_num_csn, true);
    
    gpio_put(handler->gpio_num_conv, false);

    return num_bytes;
}


int ads8881_four_wire_with_busy(ads8881_handler_t *handler, uint8_t *buffer_rx, uint8_t len){
    // --- CS goes to DIN and DOUT is used with pullup
    // Conversion Phase
    gpio_put(handler->gpio_num_conv, true);
    for(uint16_t i = 0; i < 999; i++);
    gpio_put(handler->gpio_num_csn, false);
    
    // Acquisition Phase
    while(gpio_get(handler->spi_handler->pin_miso));  
    int num_bytes = spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, len);
    gpio_put(handler->gpio_num_csn, true);
    gpio_put(handler->gpio_num_conv, false);

    return num_bytes;
}


int ads8881_rqst_data_mode(ads8881_handler_t *handler, uint8_t *data, uint8_t len){
    if(!handler->init_done){
        ads8881_init(handler);
    } else {
        switch(handler->adc_mode){
            case ADS8881_THREE_WIRE_WO_BUSY_IND:
                return ads8881_three_wire_wo_busy(handler, data, len);
            case ADS8881_THREE_WIRE_W_BUSY_IND:
                return ads8881_three_wire_with_busy(handler, data, len);
            case ADS8881_FOUR_WIRE_WO_BUSY_IND:
                return ads8881_four_wire_wo_busy(handler, data, len);
            case ADS8881_FOUR_WIRE_W_BUSY_IND:
                return ads8881_four_wire_with_busy(handler, data, len);
            default:
                return -1;
        }
    }
}


// =========================================== FUNCTIONS FOR STUPID MODE ==========================================
uint32_t ads8881_rqst_data_normal(ads8881_handler_t *handler){
    uint8_t buffer_rx[3] = {0x00};

    // Start Data Acquisition
    gpio_put(handler->gpio_num_csn, true);
    gpio_put(handler->gpio_num_conv, true);
    spi_read_blocking(handler->spi_handler->spi_mod, 0xFF, buffer_rx, 1);
    gpio_put(handler->gpio_num_csn, false);

    // Get Data
    spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, 2);
    gpio_put(handler->gpio_num_csn, true);
    gpio_put(handler->gpio_num_conv, false);

    return translate_array_into_uint32(buffer_rx, 3);
}


uint32_t ads8881_rqst_data_cheesy(ads8881_handler_t *handler){
    // --- CS goes to CONVST, MOSI to DIN and DOUT is used with pullup (stupid mode)
    uint8_t buffer_rx[3] = {0x00};

    // Conversion Phase
    gpio_put(handler->gpio_num_csn, true); // Start aquisition
    sleep_us(10); // Aquisition phase, specified: 500..710ns

    gpio_put(handler->gpio_num_csn, false);
    spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, 3);
    
    // keep CS low until next conversion starts with CS=high
    return (buffer_rx[0] << 16) | (buffer_rx[2] << 8) | (buffer_rx[3] << 0);
}


uint32_t ads8881_rqst_data(ads8881_handler_t *handler){
    if(!handler->init_done){
        ads8881_init(handler);
    } else {
        return (handler->gpio_num_conv) ? ads8881_rqst_data_normal(handler) : ads8881_rqst_data_cheesy(handler);
    }
}
