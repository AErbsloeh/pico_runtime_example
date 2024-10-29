#ifndef _VEML7700_H_
#define _VEML7700_H_

#include "lib/i2c_handler.h"


// Handler for configuring and controlling the device
typedef struct {
    i2c_device_handler_t *i2c_mod;
    uint8_t gain;
    uint8_t int_time;
    bool    init_done;
    bool    en_pwr_saving;
    bool    use_isr_thres;
} veml7700_handler_t;


/***********************************************************************************************************************************************************
 * VEML7700 COMMANDS (More infos at: https://www.vishay.com/docs/84286/veml7700.pdf)
 * **********************************************************************************************************************************************************/
#define VEML7700_ADR            0x10

#define VEML7700_GAIN_X1        0x00
#define VEML7700_GAIN_X2        0x01
#define VEML7700_GAIN_XEIGHT    0x02
#define VEML7700_GAIN_XQUAD     0x03

#define VEML7700_INT_100MS      0x00
#define VEML7700_INT_200MS      0x01
#define VEML7700_INT_400MS      0x02
#define VEML7700_INT_800MS      0x03
#define VEML7700_INT_50MS       0x08
#define VEML7700_INT_25MS       0x0C


 /***********************************************************************************************************************************************************
 * VEML7700 HANDLER FUNCTIONS
 * **********************************************************************************************************************************************************/

/** \brief Reading device ID of VEML7700 light intensity sensor
 *
 * \param handler   VEML7700 Handler for init. and processing data
 * \return True, if right device ID of sensor is available
 */
bool VEML7700_read_id(veml7700_handler_t *handler);



/** \brief Initialise VEML7700 light intensity sensor
 *
 * \param handler   VEML7700 Handler for init. and processing data
 * \param enable    Enabling or disabling the sensor
 * \return True, if initialisation of sensor was successful
 */
bool VEML7700_init(veml7700_handler_t *handler, bool enable);


/**
 * 
 */
void VEML7700_set_power_saving_mode(veml7700_handler_t *handler, uint8_t mode);


/**
 * 
 */
uint16_t VEML7700_get_als_value(veml7700_handler_t *handler);


/**
 * 
 */
uint16_t VEML7700_get_white(veml7700_handler_t *handler);

#endif
