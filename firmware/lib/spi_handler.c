#include "lib/spi_handler.h"


bool configure_spi_module(spi_device_handler_t *handler, bool use_spi_slave){
    // --- Init of GPIOs
    // GPIO: CS
    gpio_init(handler->pin_cs);
    gpio_set_dir(handler->pin_cs, GPIO_OUT);
    gpio_set_drive_strength(handler->pin_cs, GPIO_DRIVE_STRENGTH_2MA);
    gpio_put(handler->pin_cs, true);

    // GPIO: MOSI, SCLK, MISO
    gpio_set_function(handler->pin_cs, GPIO_FUNC_SPI);
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
