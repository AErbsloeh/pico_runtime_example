#ifndef _VEML7700_H_
#define _VEML7700_H_


#include "hal/i2c/i2c.h"


/***********************************************************************************************************************************************************
 * VEML7700 COMMANDS (More infos at: https://www.vishay.com/docs/84286/veml7700.pdf)
 * **********************************************************************************************************************************************************/
#define VEML7700_ADR            0x10

#define VEML7700_GAIN_X1        0x00
#define VEML7700_GAIN_X2        0x01
#define VEML7700_GAIN_X8	    0x02
#define VEML7700_GAIN_X4		0x03

#define VEML7700_INT_100MS      0x00
#define VEML7700_INT_200MS      0x01
#define VEML7700_INT_400MS      0x02
#define VEML7700_INT_800MS      0x03
#define VEML7700_INT_50MS       0x08
#define VEML7700_INT_25MS       0x0C


// Handler for configuring and controlling the device
typedef struct {
    i2c_t *i2c_mod;
    uint8_t gain;
    uint8_t int_time;
    bool    en_device;
    bool    use_isr_thres;
	bool    init_done;
} veml7700_t;


static veml7700_t VEML7700_DEFAULT_CONFIG = {
	.i2c_mod = &DEVICE_I2C_DEFAULT,
	.gain = VEML7700_GAIN_X1,
	.int_time = VEML7700_INT_100MS,
	.en_device = true,
	.use_isr_thres = false,
	.init_done = false
};


 /***********************************************************************************************************************************************************
 * VEML7700 HANDLER FUNCTIONS
 * **********************************************************************************************************************************************************/


/*! \brief Initialise VEML7700 light intensity sensor
 * \param handler   VEML7700 Handler for init. and processing data
 * \return True, if initialisation of sensor was successful
 */
bool VEML7700_init(veml7700_t *handler);


/*! \brief Function for reading VEML7700 device ID
 * \param handler   VEML7700 Handler for init. and processing data
 * \param print_id	Do print the ID into terminal
 * \return True, if right device is available
 */
bool VEML7700_read_id(veml7700_t *handler, bool print_id);


/*!	\brief Function for getting the value for Ambient Light Sensitivity (ALS) from VEML7700
 *	\param handler 	VEML7700 Handler for init. and processing data 
 */
uint16_t VEML7700_get_als_value(veml7700_t *handler);


/*!	\brief Function for getting the value for White Light Density from VEML7700
 *	\param handler 	VEML7700 Handler for init. and processing data 
 */
uint16_t VEML7700_get_white(veml7700_t *handler);

#endif
