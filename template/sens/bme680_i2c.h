#ifndef BME680_I2C_H_
#define BME680_I2C_H_


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "wrapper/i2c_handler.h"


// More Informations from sensor: https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme680-ds001.pdf
// --------------- DEFINITION ---------------
#define BME680_I2C_ADR  0x76


// Handler for configuring and controlling the environment sensor BME680 from Bosch 
typedef struct {
    i2c_device_handler_t *i2c_mod;
    bool    state_sdo_vddio;
    bool    init_done;
} bme680_i2c_handler_t;


static bme680_i2c_handler_t BME680_I2C_DEFAULT_CONFIG = {
    .i2c_mod = &DEVICE_I2C_DEFAULT,
    .state_sdo_vddio = true,
    .init_done = false
};


// --------------- CALLABLE FUNCTIONS ---------------
/*! \brief Function for reading the Chip ID of the environment sensor BME680 from Bosch
*   \param  handler     Handler for setting the device
*   \param  print_id    Boolean to print the sensor ID
*   \return             Boolean if right Chip ID of sensor is available (0xB4)
*/
bool BME680_i2c_read_id(bme680_i2c_handler_t *handler, bool print_id);


/*! \brief Function for performing a sofware reset on the environment sensor BME680 from Bosch
*   \param  handler     Handler for setting the device
*   \param  print_id    Boolean to print the sensor ID
*   \return             Boolean if right Chip ID of sensor is available (0xB4)
*/
bool BME680_i2c_soft_reset(bme680_i2c_handler_t *handler);


#endif
