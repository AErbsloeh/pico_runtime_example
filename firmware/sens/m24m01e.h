#ifndef M24M01E_H_
#define M24M01E_H_


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "wrapper/i2c_handler.h"


// More Informations from sensor: https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.st.com/resource/en/datasheet/m24m01e-f.pdf&ved=2ahUKEwiJvYa-tJSLAxWFUMMIHdQLNjMQFnoECBsQAQ&usg=AOvVaw1kkFSIEU5-hbSW2xTqZpy4
// --------------- DEFINITION ---------------
#define M24M01E_I2C_ADR  0x50


// Handler for configuring and controlling the device configuration
typedef struct {
    i2c_pico_t *i2c_mod;
    uint8_t gpio_rnw;
    bool    init_done;
} m24m01e_t;

static m24m01e_t M24M01E_DEFAULT_CONFIG = {
    .i2c_mod = &DEVICE_I2C_DEFAULT,
    .gpio_rnw = 0,
    .init_done = false
};



/*! \brief Function for initializing the EEPROM
*   \param  handler     Handler for setting the device
*   \return             Boolean if right Chip ID of sensor is available (0xB4)
*/
bool M24M01E_init(m24m01e_t *handler);


/*! \brief Function for initializing the EEPROM
*   \param  handler     Handler for setting the device
*   \return             Boolean if right Chip ID of sensor is available (0xB4)
*/
bool M24M01E_read_data(m24m01e_t *handler);


/*! \brief Function for initializing the EEPROM
*   \param  handler     Handler for setting the device
*   \return             Boolean if right Chip ID of sensor is available (0xB4)
*/
bool M24M01E_write_data(m24m01e_t *handler);


/*! \brief Function for initializing the EEPROM
*   \param  handler     Handler for setting the device
*   \return             Boolean if right Chip ID of sensor is available (0xB4)
*/
bool M24M01E_delete_flash(m24m01e_t *handler);



#endif
