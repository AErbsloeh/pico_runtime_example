#ifndef AD528X_H_
#define AD528X_H_


#include "hal/i2c/i2c.h"


// More informations on: https://www.analog.com/media/en/technical-documentation/data-sheets/AD5280_5282.pdf
// ================================= DEFINITIONS =================================
#define AD528X_BASIC_I2C_ADR    0x2C

#define AD528X_GPIO_NONE        0x00
#define AD528X_GPIO_PIN0        0x01
#define AD528X_GPIO_PIN1        0x02


/*! \brief Struct handler for configuring the Digital Potentiometer AD5280/2 from Analog Devices
* \param i2c_handler    Predefined I2C handler for RP2040
* \param model_sel      Adress bits for selecting the right adress
* \param shutdown       Boolean if device should be shutdown in software
* \param gpio_state     Uint8_t value for saaving the GPIO state 
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    i2c_t *i2c_handler;
    uint8_t mode_sel;
    bool shutdown;
    uint8_t gpio_state;
    bool init_done;
} ad528x_t;


static ad528x_t AD528X_DEFAULT_CONFIG = {
    .i2c_handler = &DEVICE_I2C_DEFAULT,
    .mode_sel = 0,
    .shutdown = true,
    .gpio_state = false,
    .init_done = false
};


// ================================= FUNCTIONS =================================
/*! \brief Function for configuring the Digital Potentiometer AD5280/2 from Analog Devices
* \param handler    Device handler for using device
* \return           Boolean if initialization is done completely
*/
bool ad528x_init(ad528x_t *handler);


/*! \brief Function for doing a software reset to midscale
* \param handler    Device handler for using device
* \return           Boolean if process is done completely
*/
bool ad528x_soft_reset(ad528x_t *handler);


/*! \brief Function for controlling the shutdown of device
* \param handler    Device handler for using device
* \return           Boolean if process is done completely
*/
bool ad528x_define_shutdown(ad528x_t *handler);


/*! \brief Function for controlling the GPIO output pins of device
* \param handler    Device handler for using device
* \return           Boolean if process is done completely
*/
bool ad528x_define_gpio_output(ad528x_t *handler);


/*! \brief Function for doing a software reset to midscale
* \param handler    Device handler for using device
* \param chnnl_b    Selecting the output channel (false: CHA, true: CHB [if available])
* \param data       Uint8_t value with position of channel
* \return           Boolean if process is done completely
*/
bool ad528x_define_output(ad528x_t *handler, bool chnnl_b, uint8_t data);


#endif
