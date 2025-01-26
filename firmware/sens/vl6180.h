#ifndef VL6180_H_
#define VL6180_H_


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "lib/i2c_handler.h"


// More Informations from sensor: https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.st.com/resource/en/datasheet/vl6180.pdf&ved=2ahUKEwjL9YiCrZSLAxUDSvEDHYZQFuwQFnoECBAQAQ&usg=AOvVaw0EYhww0GCbs05jcovK_Ly2
// --------------- DEFINITION ---------------
#define VL6180_I2C_ADR  0x29


// Handler for configuring and controlling the device configuration
typedef struct {
    i2c_device_handler_t *i2c_mod;
    bool    init_done;
} vl6180_handler_t;

static vl6180_handler_t VL6180_DEFAULT_CONFIG = {
    .i2c_mod = &DEVICE_I2C_DEFAULT,
    .init_done = false
};


// --------------- CALLABLE FUNCTIONS ---------------
/*! \brief Function for reading the Chip ID
*   \param  handler     Handler for setting the device
*   \param  print_id    Boolean to print the sensor ID
*   \return             Boolean if right Chip ID of sensor is available (0xB4)
*/
bool VL6180_read_id(vl6180_handler_t *handler, bool print_id);


// --------------- CALLABLE FUNCTIONS ---------------
/*! \brief Function for initialization of
*   \param  handler     Handler for setting the device
*   \return             Boolean if right Chip ID of sensor is available (0x24)
*/
bool VL6180_init(vl6180_handler_t *handler);


#endif
