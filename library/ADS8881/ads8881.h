#ifndef ADS8881_H_
#define ADS8881_H_


#include <stdio.h>
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "wrapper/spi_handler.h"


// More informations on: https://www.ti.com/lit/ds/symlink/ads8881.pdf?ts=1738727064904&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS8881
// ================================= DEFINITIONS =================================
/*! \brief Struct handler for configuring the Analog-Digital-Converter ADS8881 from Texas Instruments
* \param spi_handler    Predefined SPI handler for RP2040
* \param gpio_num_csn   GPIO Pin for Chip Select
* \param gpio_num_csn   GPIO Pin for Converstion Start (active high)
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    spi_device_handler_t *spi_handler;
    uint8_t gpio_num_csn;
    uint8_t gpio_num_convst;
    bool init_done;
} ads8881_handler_t;


static ads8881_handler_t ADS8881_DEFAULT_CONFIG = {
    .spi_handler = &DEVICE_SPI_DEFAULT,
    .gpio_num_csn = PICO_DEFAULT_SPI_CSN_PIN,
    .gpio_num_convst = 0,
    .init_done = false
};


// ================================= FUNCTIONS =================================
/*! \brief Function for configuring the Analog-Digital-Converter ADS8881 from Texas Instruments
* \param handler    Device handler for using device
* \return           Boolean if initialization is done completely
*/
bool ads8881_init(ads8881_handler_t *handler);


/*! \brief Function for requesting data (on the cheesy way) from Analog-Digital-Converter ADS8881
* \param handler    Device handler for using device
* \return           ADC data
*/
uint32_t ads8881_rqst_data(ads8881_handler_t *handler);


#endif
