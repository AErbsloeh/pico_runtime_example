#ifndef IS66WVS1_H_
#define IS66WVS1_H_


#include <stdio.h>
#include "lib/spi_handler.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"


/*! \brief Struct handler for configuring the EEPROM IS66WVS1 using SPI interface
*   \param spi_handler      SPI Handler (predefined from lib/spi_handler.h)
*   \param gpio_num_csn     GPIO number of used Chip Select Pin (CSN)
*   \param init_done            
*/
typedef struct{
    spi_device_handler_t spi_handler;
    uint8_t gpio_num_csn;
    bool init_done; 
} is66wvs1_handler_t;


static spi_device_handler_t IS66WVS1_DEFAULT_SPI = DEVICE_SPI_DEFAULT;
IS66WVS1_DEFAULT_SPI.mode = 1;


static is66wvs1_handler_t IS66WVS1_DEFAULT_HANDLER = {
    .spi_handler = &IS66WVS1_DEFAULT_SPI;
    .gpio_num_csn = PICO_DEFAULT_SPI_CSN_PIN;
    .init_done = false;
}


#endif
