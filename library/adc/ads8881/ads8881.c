#include "adc/ads8881/ads8881.h"
#include "hardware/gpio.h"


// ================================= INTERNAL FUNCTIONS =================================
uint32_t convert_one_drqst_into_value(uint8_t *data, bool invert){
    uint32_t value = 0x00020000 ^ ((data[0] << 10) | (data[1] << 2) | (data[2] >> 6));
    return (invert) ? 0x00020000 - (value - 0x00020000) : value;
}


int ads8881_three_wire_wo_busy(ads8881_t *handler, uint8_t *buffer_rx, uint8_t len){
    // --- DIN should be tied to VDDD and DOUT is used without pullup (normal DOUT is tri-state)
    // Conversion Phase
    gpio_put(handler->gpio_num_cnv, true);
    sleep_us(1);   
    gpio_put(handler->gpio_num_cnv, false);

    // Acquisition Phase
    return spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, len);
}


int ads8881_three_wire_with_busy(ads8881_t *handler, uint8_t *buffer_rx, uint8_t len){
    // --- DIN should be tied to VDDD and DOUT is used with pullup
    // Conversion Phase
    gpio_put(handler->gpio_num_cnv, true);
    for(uint16_t i = 0; i < 999; i++);
    gpio_put(handler->gpio_num_cnv, false);

    // Acquisition Phase
    while(gpio_get(handler->spi_handler->pin_miso));  
    return spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, len);
}


int ads8881_four_wire_wo_busy(ads8881_t *handler, uint8_t *buffer_rx, uint8_t len){
    // --- CS goes to DIN and DOUT is used without pullup (normal DOUT is tri-state)
    // Conversion Phase
    gpio_put(handler->gpio_num_cnv, true);
    gpio_put(handler->gpio_num_din, true);
    sleep_us(1);   
    
    // Acquisition Phase
    gpio_put(handler->gpio_num_din, false);
    int num_bytes = spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, len);
    gpio_put(handler->gpio_num_din, true);
    
    gpio_put(handler->gpio_num_cnv, false);

    return num_bytes;
}


int ads8881_four_wire_with_busy(ads8881_t *handler, uint8_t *buffer_rx, uint8_t len){
    // --- CS goes to DIN and DOUT is used with pullup
    // Conversion Phase
    gpio_put(handler->gpio_num_cnv, true);
    for(uint16_t i = 0; i < 999; i++);
    gpio_put(handler->gpio_num_din, false);
    
    // Acquisition Phase
    while(gpio_get(handler->spi_handler->pin_miso));  
    int num_bytes = spi_read_blocking(handler->spi_handler->spi_mod, 0x00, buffer_rx, len);
    gpio_put(handler->gpio_num_din, true);
    gpio_put(handler->gpio_num_cnv, false);

    return num_bytes;
}


// ================================= CALLABLE FUNCTIONS =================================
bool ads8881_init(ads8881_t *handler){
    if(!handler->spi_handler->init_done){
        configure_spi_module(handler->spi_handler, false);
    };

    if(!handler->spi_handler->mode == 0){
        // ADC SPI Mode is not 0
        handler->init_done = false;
    } else {
        // --- Inif of CONVST pin
        gpio_init(handler->gpio_num_cnv);
        gpio_set_dir(handler->gpio_num_cnv, GPIO_OUT);
        gpio_put(handler->gpio_num_cnv, false);
        handler->init_done = true;

        if(handler->adc_mode > 1){
            // --- Init of CS pin
            gpio_init(handler->gpio_num_din);
            gpio_set_dir(handler->gpio_num_din, GPIO_OUT);
            gpio_set_drive_strength(handler->gpio_num_din, GPIO_DRIVE_STRENGTH_2MA);
            gpio_put(handler->gpio_num_din, false);
        }
    }    
    return handler->init_done;
}


int ads8881_rqst_data_mode(ads8881_t *handler, uint8_t *data, uint8_t len){
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


uint32_t ads8881_rqst_data(ads8881_t *handler){
    if(!handler->init_done){
        ads8881_init(handler);
    } else {
        uint8_t data[3];
        ads8881_rqst_data_mode(handler, data, 3);
        return convert_one_drqst_into_value(data, handler->invert_out);
    }
}
