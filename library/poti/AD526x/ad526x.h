#ifndef AD526X_H_
#define AD526X_H_


#include "hal/spi/spi.h"


// More informations on: https://www.analog.com/media/en/technical-documentation/data-sheets/AD5260_5262.pdf
// ================================= DEFINITIONS =================================

/*! \brief Struct handler for configuring the Digital Potentiometer AD5260/2 from Analog Devices
* \param spi_handler    Predefined I2C handler for RP2040
* \param gpio_cs        Selected GPIO pin for using Chip-Select (CS)
* \param device_mode    Selected device mode (false: AD5260, true: AD5262)
* \param shutdown       Boolean if device should be shutdown in software
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    spi_t *spi_handler;
    uint8_t gpio_cs;
    bool device_mode;
    bool shutdown;
    bool init_done;
} ad526x_t;


static ad526x_t AD526X_DEFAULT_CONFIG = {
    .spi_handler = &DEVICE_SPI_DEFAULT,
    .gpio_cs = PICO_DEFAULT_SPI_CSN_PIN,
    .device_mode = false,
    .shutdown = true,
    .init_done = false
};


// ================================= FUNCTIONS =================================
/*! \brief Function for configuring the Digital Potentiometer AD5260/2 from Analog Devices
* \param handler    Device handler for using device
* \return           Boolean if initialization is done completely
*/
bool ad526x_init(ad526x_t *handler);


/*! \brief Function for doing a software reset to midscale
* \param handler    Device handler for using device
* \return           Boolean if process is done completely
*/
bool ad526x_soft_reset(ad526x_t *handler);


/*! \brief Function for controlling the shutdown of device
* \param handler    Device handler for using device
* \return           Boolean if process is done completely
*/
bool ad526x_define_shutdown(ad526x_t *handler);


/*! \brief Function for doing a software reset to midscale
* \param handler    Device handler for using device
* \param chnnl      Selecting the output channel (false: CHA, true: CHB [if available])
* \param data       Uint8_t value with position of channel
* \return           Boolean if process is done completely
*/
bool ad526x_define_output(ad526x_t *handler, bool chnnl, uint8_t data);


#endif
