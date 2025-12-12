#ifndef _SHT21_H_
#define _SHT21_H_


#include "hal/i2c/i2c.h"


/* **********************************************************************************************************************************************************
 * SHT21 COMMAND SET
 *
 * T = Temperature
 * RH = Relative humidity
 * **********************************************************************************************************************************************************
 */
#define SHT21_T_HOLD     0xE3 // Trigger T measurement (hold master)
#define SHT21_RH_HOLD    0xE5 // Trigger RH measurement (hold master)
#define SHT21_T_NO_HOLD  0xF3 // Trigger T measurement (no hold master)
#define SHT21_RH_NO_HOLD 0xF5 // Trigger RH measurement (no hold master)
#define SHT21_USER_REG_W 0xE6 // Write user register
#define SHT21_USER_REG_R 0xE7 // Read user register
#define SHT21_RESET      0xFE // Soft reset

#define SHT21_HEATER_ON   true  // Enable on-chip heater
#define SHT21_HEATER_OFF  false // Disable on-chip heater
#define SHT21_ENABLE_OTP  false // Enable OTP reload (not recommended, use soft reset instead)
#define SHT21_DISABLE_OTP true  // Disable OTP reload

/**
 * **********************************************************************************************************************************************************
 * SHT21 MEASUREMENT RESOLUTION
 *
 *  The measurement resolution is set in the user register:
 *  user register bit 0 = T resolution
 *  user register bit 7 = RH resolution
 * **********************************************************************************************************************************************************
 */
#define SHT21_RESOLUTION_12_14 0x00 // RH 12 bit (0), T 14 bit (0)
#define SHT21_RESOLUTION_08_12 0x01 // RH  8 bit (0), T 12 bit (1)
#define SHT21_RESOLUTION_10_13 0x80 // RH 10 bit (1), T 13 bit (0)
#define SHT21_RESOLUTION_11_11 0x81 // RH 11 bit (1), T 11 bit (1)


/**
 * **********************************************************************************************************************************************************
 * SHT21 HANDLER 
 * **********************************************************************************************************************************************************
 */
typedef struct {
    i2c_t *i2c_mod;
    bool    heater_enable;
    bool    otp_enable;
    uint8_t resolution;
    bool    init_done;
} sht21_t;

static sht21_t SHT21_CONFIG_DEFAULT = {
	.i2c_mod = &DEVICE_I2C_DEFAULT,
	.heater_enable = SHT21_HEATER_OFF,
	.otp_enable = SHT21_DISABLE_OTP,
	.resolution = SHT21_RESOLUTION_12_14,
	.init_done = false
};


 /***********************************************************************************************************************************************************
 * SHT21 HANDLER FUNCTIONS
 * **********************************************************************************************************************************************************/
/** \brief Initialise SHT21 humidity and temperature sensor
 *
 * \param handler   SHT21 Handler for init. and processing data
 * \return True, if initialisation of SHT21 sensor was successful
 */
bool SHT21_init(sht21_t *handler);


/** \brief Performing a soft reset of the SHT21
 *
 * \param handler   SHT21 Handler for init. and processing data
 */
void SHT21_do_soft_reset(sht21_t *handler);


/** \brief Read data and calculate humidity from SHT21 sensor
 *
 * \param handler   SHT21 Handler for init. and processing data
 * \return Relative humidity [%]
 */
float SHT21_get_humidity_float(sht21_t *handler);


/** \brief Read humidity data from SHT21 sensor
 *
 * \param handler   SHT21 Handler for init. and processing data
 * \return Relative humidity (without calcuting, only rawdata)
 */
uint16_t SHT21_get_humidity_uint(sht21_t *handler);


/** \brief Read data and calculate temperature from SHT21 sensor
 *
 * \param handler   SHT21 Handler for init. and processing data
 * \return Temperature [K]
 */
float SHT21_get_temperature_float(sht21_t *handler);


/** \brief Read temperature data from SHT21 sensor
 *
 * \param handler   SHT21 Handler for init. and processing data
 * \return Temperature (without calcuting, only rawdata)
 */
uint16_t SHT21_get_temperature_uint(sht21_t *handler);

#endif
