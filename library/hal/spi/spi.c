#include "hal/spi/spi.h"
#include "hardware/gpio.h"


// ========================================= DEFINITION FOR NORMAL SPI ===============================================
bool configure_spi_module(spi_t *handler, bool use_spi_slave){
    // --- Init of GPIOs
    // GPIO: MOSI, SCLK, MISO
    gpio_set_function(handler->pin_sclk, GPIO_FUNC_SPI);
    gpio_set_function(handler->pin_mosi, GPIO_FUNC_SPI);
    gpio_set_function(handler->pin_miso, GPIO_FUNC_SPI);

    // --- Init of SPI module
    spi_init(handler->spi_mod, handler->fspi_khz * 1000);
    spi_set_slave(handler->spi_mod, use_spi_slave);
    spi_set_format(
        handler->spi_mod,   
        handler->bits_per_transfer, 
        (handler->mode >= 2) ? SPI_CPOL_1 : SPI_CPOL_0,
        (handler->mode % 2 == 1) ? SPI_CPHA_1 : SPI_CPHA_0,
        (handler->msb_first) ? SPI_MSB_FIRST : SPI_LSB_FIRST
    );

    handler->init_done = true;
    return handler->init_done;
}


int8_t send_data_spi_module(spi_t *handler, uint8_t gpio_num_csn, uint8_t data_tx[], size_t length){
    gpio_put(gpio_num_csn, false);
    int8_t status = spi_write_blocking(handler->spi_mod, data_tx, length);
    gpio_put(gpio_num_csn, true);
    return status;
}


int8_t receive_data_spi_module(spi_t *handler, uint8_t gpio_num_csn, uint8_t data_tx[], uint8_t data_rx[], size_t length){
    gpio_put(gpio_num_csn, false);
    int8_t status = spi_write_read_blocking(handler->spi_mod, data_tx, data_rx, length);
    gpio_put(gpio_num_csn, true);
    return status;
}


// ========================================= DEFINITION FOR SOFT SPI ===============================================
bool configure_spi_module_soft(spi_t *handler){
    // GPIO: SCLK
    gpio_init(handler->pin_sclk);
    gpio_set_dir(handler->pin_sclk, GPIO_OUT);
    gpio_set_drive_strength(handler->pin_sclk, GPIO_DRIVE_STRENGTH_2MA);
    gpio_put(handler->pin_sclk, (handler->mode == 2) || (handler->mode == 3));

    // GPIO: MOSI
    gpio_init(handler->pin_mosi);
    gpio_set_dir(handler->pin_mosi, GPIO_OUT);
    gpio_set_drive_strength(handler->pin_mosi, GPIO_DRIVE_STRENGTH_2MA);
    gpio_put(handler->pin_mosi, false);

    // GPIO: MISO
    gpio_init(handler->pin_miso);
    gpio_set_dir(handler->pin_miso, GPIO_IN);
    gpio_set_input_hysteresis_enabled(handler->pin_miso, true);
    gpio_set_drive_strength(handler->pin_miso, GPIO_DRIVE_STRENGTH_2MA);

    handler->init_done = true;
    return handler->init_done;
}


uint16_t send_data_spi_module_soft(spi_t *handler, uint8_t gpio_num_csn, uint16_t data){
    uint16_t data_returned = 0;
    uint8_t position_send = (handler->msb_first) ? (uint8_t)handler->bits_per_transfer-1 : 0;
    bool cpol = (handler->mode == 2) || (handler->mode == 3);
    bool cpha = (handler->mode % 2 == 1);
    
    gpio_put(gpio_num_csn, false);
    for(uint8_t cnt=0; cnt < handler->bits_per_transfer; cnt++){
        // Starting condition
        if(cpha && cnt == 0) {
            sleep_us(1);
        } else {
            gpio_put(handler->pin_mosi, (data >> position_send) & 0x0001);
        };

        // First edge
        gpio_put(handler->pin_sclk, !cpol);
        if(cpha){
            gpio_put(handler->pin_mosi, (data >> position_send) & 0x0001);
        } else {
            data_returned |= (gpio_get(handler->pin_miso) << position_send);
        };
        sleep_us(1);

        // Second edge
        gpio_put(handler->pin_sclk, cpol);
        if(cpha){
            data_returned |= (gpio_get(handler->pin_miso) << position_send);
        } else {
            gpio_put(handler->pin_mosi, cnt % 2 == 1);
        };
        sleep_us(1);
        position_send = (handler->msb_first) ? position_send - 1 : position_send + 1;
    };
    gpio_put(gpio_num_csn, true);
    gpio_put(handler->pin_sclk, cpol);

    return data_returned;
}


uint32_t translate_array_into_uint32(uint8_t buffer_rx[], size_t len_rx){
    uint32_t raw_data = 0;
    for(uint8_t idx = 0; idx < len_rx; idx++){
        raw_data |= buffer_rx[idx] << 8*idx;
    }
    return raw_data;
}
