/** \file SHT21.c
 * \brief SHT21 humidity and temperature sensor
 *
 * For further detail on the SHT21 humidity and temperature sensor see
 * https://www.farnell.com/datasheets/1780639.pdf
 */

#include "main.h"

uint8_t           buffer[32];
i2c_inst_t*       i2c_mode;
static const long RES_CALC_FACTOR = 1 << 16; // Factor for calculation of temperature and relative humidity

bool SHT21_init(i2c_inst_t* i2c, bool heater_enabled, bool otp_disabled, uint8_t resolution)
{
    uint8_t register_data[2];
    i2c_mode = i2c;

    // Read user register from sensor
    buffer[0] = SHT21_USER_REG_R;
    i2c_write_blocking(i2c_mode, SHT21_ADDRESS, buffer, 1, true);
    i2c_read_blocking(i2c_mode, SHT21_ADDRESS, buffer, 1, false);
    // Load write command and current user register content to buffer to be written
    register_data[0] = SHT21_USER_REG_W;
    register_data[1] = buffer[0];

    // Set user register data to be written
    if (heater_enabled) {
        register_data[1] |= SHT21_ENABLE_HEATER_MASK; // Mask bit 2 to 1
    } else {
        register_data[1] &= ~SHT21_ENABLE_HEATER_MASK; // Mask bit 2 to 0
    }

    if (otp_disabled) {
        register_data[1] |= SHT21_DISABLE_OTP_MASK; // Mask bit 1 to 1
    } else {
        register_data[1] &= ~SHT21_DISABLE_OTP_MASK; // Mask bit 1 to 0
    }

    switch (resolution) {
        // Measurement resolution is set in bit 0 (T) and bit 7 (RH) in the user register
        case SHT21_RESOLUTION_12_14:
            register_data[1] &= 0x7E; // Mask bits 0 and 7 to 0
            break;
        case SHT21_RESOLUTION_08_12:
            register_data[1] &= 0x7F; // Mask bit 7 to 0...
            register_data[1] |= 0x01; // ... and bit 0 to 1
            break;
        case SHT21_RESOLUTION_10_13:
            register_data[1] &= 0xFE; // Mask bit 0 to 0...
            register_data[1] |= 0x80; // ... and bit 7 to 1
            break;
        case SHT21_RESOLUTION_11_11:
            register_data[1] |= 0x81; // Mask bits 0 and 7 to 1
            break;
        default:
            // Default measurement resolution is 12 bit RH and 14 bit T
            register_data[1] &= 0x7E; // Mask bits 0 and 7 to 0
            break;
    }

    // Send user resgister data to sensor
    if (i2c_write_blocking(i2c_mode, SHT21_ADDRESS, register_data, 2, false) == PICO_ERROR_GENERIC) {
        return false;
    }

    return true;
}

bool SHT21_start_measurement_no_hold(uint8_t command)
{
    buffer[0]   = command;
    int success = i2c_write_blocking(i2c_mode, SHT21_ADDRESS, buffer, 1, false);
    // Check if error occured while writing data (negative error codes)
    if (success <= -1) {
        return false;
    } else {
        return true;
    }
}

static inline uint16_t SHT21_read_data_no_hold()
{
    uint16_t raw_data;
    uint8_t  buffer[3];
    while (i2c_read_blocking(i2c_mode, SHT21_ADDRESS, buffer, 3, false) == PICO_ERROR_GENERIC)
        ;
    raw_data = ((buffer[0] << 8) + buffer[1]) & 0xFFFC; // Keep only first 14 data bits, ignore last two status bits and checksum
    return raw_data;
}

float SHT21_get_humidity_no_hold()
{
    uint16_t raw_data     = SHT21_read_data_no_hold();
    float    rel_humidity = -6 + (125 * raw_data) / RES_CALC_FACTOR;
    return rel_humidity;
}

float SHT21_get_temperature_no_hold()
{
    uint16_t raw_data    = SHT21_read_data_no_hold();
    float    temperature = -46.85 + (175.72 * raw_data) / RES_CALC_FACTOR;
    return temperature;
}
