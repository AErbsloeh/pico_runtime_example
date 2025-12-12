#ifndef ADGS1208_H_
#define ADGS1208_H_


#include "pico/stdlib.h"
#include "hal/spi/spi.h"


// More informations on: https://www.analog.com/media/en/technical-documentation/data-sheets/adgs1208-1209.pdf
// ================================= DEFINITIONS =================================
#define ADGS1208_SPI_MODE0 0
#define ADGS1208_SPI_MODE3 3


/*! \brief Struct handler for configuring the Multiplexer ADGS120X from Analog Devices with SPI interface
* \param spi_handler    Predefined SPI handler for RP2040
* \param gpio_num_csn   GPIO Pin for Chip Select
* \param num_device_dc  Number of devices which are daisy chained (< 1 will be activated)
* \param use_rstn_hw    Boolean if hardware reset line is used 
* \param gpio_num_rstn  GPIO Pin for Reset Line (optional)
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    spi_t *spi_handler;
    uint8_t gpio_num_csn;
    uint8_t num_device_dc;
    bool use_rstn_hw;
    uint8_t gpio_num_rstn;
    bool gpio_1_state;
    bool gpio_2_state;
    bool gpio_3_state;
    bool gpio_4_state;
    bool init_done;
} adgs1208_t;


static adgs1208_t ADGS1208_SETTINGS_DEFAULT = {
    .spi_handler = &DEVICE_SPI_DEFAULT,
    .gpio_num_csn = PICO_DEFAULT_SPI_CSN_PIN,
    .num_device_dc = 1,
    .use_rstn_hw = false,
    .gpio_num_rstn = 0,
    .gpio_1_state = false,
    .gpio_2_state = false,
    .gpio_3_state = false,
    .gpio_4_state = false,
    .init_done = false
};


// More informations on: https://www.analog.com/media/en/technical-documentation/data-sheets/adgs1208-1209.pdf
// ================================= FUNCTIONS =================================
/*! \brief Function for configuring the Multiplexer ADGS1208 from Analog Devices 
* \param config_device  Device handler for using device
* \return               Boolean if configuration is done and device is available
*/
bool adgs1208_init(adgs1208_t *config_device);


/*! \brief Function for resetting the MUX values 
* \param config_device  Device handler for using device
*/
void adgs1208_reset(adgs1208_t *config_device);


/*! \brief Function for enabling the Daisy Chain Mode 
* \param config_device  Device handler for using device
*/
void adgs1208_enable_daisy_chain(adgs1208_t *config_device);


/*! \brief Function for defining the MUX output 
* \param config_device  Device handler for using device
* \param mux_data       Array with data for defining the position [val_0, ..., val_N] and each value is defined with 0: disable, 1: S1, ... 8: S8
* \return               Boolean if output definition is successful done
*/
bool adgs1208_define_output(adgs1208_t *config_device, const uint8_t *mux_data);


#endif
