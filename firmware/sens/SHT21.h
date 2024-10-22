/** \file SHT21.h
 * \brief Header for SHT21 humidity and temperature sensor
 *
 * For further detail on the SHT21 humidity and temperature sensor see
 * https://www.farnell.com/datasheets/1780639.pdf
 */

#ifndef _SHT21_H_
#define _SHT21_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SHT21_ADDRESS 0x40

/**
 * **********************************************************************************************************************************************************
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

#define SHT21_HEATER_ON   true  // Enable on-chip heater
#define SHT21_HEATER_OFF  false // Disable on-chip heater
#define SHT21_ENABLE_OTP  false // Enable OTP reload (not recommended, use soft reset instead)
#define SHT21_DISABLE_OTP true  // Disable OTP reload

// Configuration bits in user register (for bitmasking)
typedef enum SHT21_USER_REG_enum {
    SHT21_BATTERY_LOW_MASK   = (0x01 << 6), // End of battery indicator (VDD < 2,25V), read only
    SHT21_ENABLE_HEATER_MASK = (0x01 << 2), // Enable on-chip heater
    SHT21_DISABLE_OTP_MASK   = (0x01 << 1), // Disable OTP reload
} SHT21_USER_REG_MASKS_t;

/**
 * **********************************************************************************************************************************************************
 * SHT21 HANDLER FUNCTIONS
 * **********************************************************************************************************************************************************
 */

/** \brief Initialise SHT21 humidity and temperature sensor
 *
 * \param i2c I2C instance
 * \param heater_enabled True, if on-chip heater shall be enabled
 * \param otp_disabled True, if OTP reload shall be disabled
 * \param resolution Measurement resolution of RH and T
 * \return True, if initialisation of SHT21 sensor was successful
 */
bool SHT21_init(i2c_inst_t* i2c, bool heater_enabled, bool otp_disabled, uint8_t resolution);

/** \brief Start SHT21 measurement with no master hold
 *
 * \param command Command code
 */
bool SHT21_start_measurement_no_hold(uint8_t command);

/** \brief Read data from SHT21 with no master hold
 *
 * \param command Command code
 */
static inline uint16_t SHT21_read_data_no_hold();

/** \brief Read data and calculate humidity from SHT21 sensor
 *
 * \return Relative humidity [%]
 */
float SHT21_get_humidity_no_hold();

/** \brief Read data and calculate temperature from SHT21 sensor
 *
 * \return Temperature [°C]
 */
float SHT21_get_temperature_no_hold();

#ifdef __cplusplus
}
#endif

#endif
