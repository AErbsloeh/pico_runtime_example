#ifndef VL6180_H_
#define VL6180_H_


#include "hal/i2c/i2c.h"


// More Informations from sensor: https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.st.com/resource/en/datasheet/vl6180.pdf&ved=2ahUKEwjL9YiCrZSLAxUDSvEDHYZQFuwQFnoECBAQAQ&usg=AOvVaw0EYhww0GCbs05jcovK_Ly2
// --------------- DEFINITION ---------------
#define VL6180X_I2C_ADR  0x29

#define VL6180X_ALS_GAIN_1 0x06    ///< 1x gain
#define VL6180X_ALS_GAIN_1_25 0x05 ///< 1.25x gain
#define VL6180X_ALS_GAIN_1_67 0x04 ///< 1.67x gain
#define VL6180X_ALS_GAIN_2_5 0x03  ///< 2.5x gain
#define VL6180X_ALS_GAIN_5 0x02    ///< 5x gain
#define VL6180X_ALS_GAIN_10 0x01   ///< 10x gain
#define VL6180X_ALS_GAIN_20 0x00   ///< 20x gain
#define VL6180X_ALS_GAIN_40 0x07   ///< 40x gain


// Handler for configuring and controlling the device configuration
typedef struct {
    i2c_t *i2c_mod;
    uint8_t max_convergence_ms;
    bool    init_done;
} vl6180_t;

static vl6180_t VL6180_DEFAULT_CONFIG = {
    .i2c_mod = &DEVICE_I2C_DEFAULT,
    .max_convergence_ms = 1,
    .init_done = false
};


// --------------- CALLABLE FUNCTIONS ---------------
/*! \brief Function for reading the Chip ID
*   \param  handler     Handler for setting the device
*   \param  print_id    Boolean to print the sensor ID
*   \return             Boolean if right Chip ID of sensor is available (0xB4)
*/
bool VL6180_read_id(vl6180_t *handler, bool print_id);


/*! \brief Function for initialization of Proximity sensing module VL6180 from STmicroelectronics 
*   \param  handler     Handler for setting the device
*   \return             Boolean if right Chip ID of sensor is available (0x24)
*/
bool VL6180_init(vl6180_t *handler);


/*! \brief Function for set the scaling value of device
*   \param  handler             Handler for setting the device
*   \param new_scaling_value    Scaling value (1 .. 3)
*/
void VL6180_set_scaling(vl6180_t *handler, uint16_t new_scaling_value);


/*! \brief Function for reading the range error register of VL6180
*   \param  handler     Handler for setting the device
*   \return             Error code of 0x004D register
*/
uint8_t VL6180_get_range_error(vl6180_t *handler);


/*! \brief Function for reading the range error ISR register of VL6180
*   \param  handler     Handler for setting the device
*   \return             Error code of 0x004F register
*/
uint8_t VL6180_get_range_error_isr(vl6180_t *handler);


/*! \brief Function for triggering the single measurement method
*   \param  handler     Handler for setting the device
*   \return             Boolean with transmission is done without error
*/
bool VL6180_start_single_measurement(vl6180_t *handler);


/*! \brief Function for triggering the continous measurement method
*   \param  handler     Handler for setting the device
*   \return             Boolean with transmission is done without error
*/
bool VL6180_start_cont_measurement(vl6180_t *handler);


/*! \brief Function for stopping the continous measurement method
*   \param  handler     Handler for setting the device
*   \return             Boolean with transmission is done without error
*/
bool VL6180_stop_cont_measurement(vl6180_t *handler);


/*! \brief Function for reading the range value
*   \param  handler     Handler for setting the device
*   \return             uint8_t value of distance in mm (0...255 mm)
*/
uint8_t VL6180_get_range_value(vl6180_t *handler);


#endif
