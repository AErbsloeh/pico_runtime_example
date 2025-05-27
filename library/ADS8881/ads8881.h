#ifndef ADS8881_H_
#define ADS8881_H_


#include <stdio.h>
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "wrapper/spi_handler.h"


// More informations on: https://www.ti.com/lit/ds/symlink/ads8881.pdf?ts=1738727064904&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FADS8881
// ================================= DEFINITIONS =================================
#define ADS8881_THREE_WIRE_W_BUSY_IND   0
#define ADS8881_THREE_WIRE_WO_BUSY_IND  1
#define ADS8881_FOUR_WIRE_W_BUSY_IND    2
#define ADS8881_FOUR_WIRE_WO_BUSY_IND   3


/*! \brief Struct handler for configuring the Analog-Digital-Converter ADS8881 from Texas Instruments
* \param spi_handler    Predefined SPI handler for RP2040
* \param gpio_num_csn   GPIO Pin for Chip-Select (CS)
* \param gpio_num_conv  GPIO Pin for Converstion Start (active high)
* \param adc_mode       Integer to identify the ADC mode (0: 3-wire with busy indication, 1: 3-wire without busy indication, 2: 4-wire with busy indication, 3: 4-wire without busy indication)
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    spi_pico_t *spi_handler;
    uint8_t gpio_num_csn;
    uint8_t gpio_num_conv;
    uint8_t adc_mode;
    bool init_done;
} ads8881_t;


static ads8881_t ADS8881_DEFAULT_CONFIG = {
    .spi_handler = &DEVICE_SPI_DEFAULT,
    .gpio_num_csn = PICO_DEFAULT_SPI_CSN_PIN,
    .gpio_num_conv = 0,
    .adc_mode = ADS8881_THREE_WIRE_WO_BUSY_IND,
    .init_done = false
};


// ================================= FUNCTIONS =================================
/*! \brief Function for configuring the Analog-Digital-Converter ADS8881 from Texas Instruments
* \param handler    Device handler for using device
* \return           Boolean if initialization is done completely
*/
bool ads8881_init(ads8881_t *handler);


/*! \brief Function for requesting data (on the cheesy way) from Analog-Digital-Converter ADS8881
* \param handler    Device handler for using device
* \return           ADC data
*/
uint32_t ads8881_rqst_data(ads8881_t *handler);


/*! \brief Function for requesting data from Analog-Digital-Converter ADS8881 (described in datasheet)
* \param handler    Device handler for using device
* \param data       Pointer to data array (uint8_t) for receiving data (with length of 18 bits x number of devices, rounded up to byte)
* \param len        Length of bytes in data array (uint8_t)
* \return           ADC data
*/
int ads8881_rqst_data_mode(ads8881_t *handler, uint8_t *data, uint8_t len);


#endif
