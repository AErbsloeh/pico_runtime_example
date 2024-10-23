#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


/*! \brief Struct handler for configuring the I2C interface of RP2040
* \param pin_sda    GPIO num of used SDA
* \param pin_scl    GPIO num of used SCL
* \param spi_mod    I2C handler of RP2040 (i2c0 or i2c1)
* \param fi2c_khz   Used I2C clock [in kHz]
* \param init_done  Boolean if configuration is done        
*/
typedef struct{
    uint8_t pin_sda;
    uint8_t pin_scl;
    i2c_inst_t *i2c_mod;
    uint16_t fi2c_khz;
    bool init_done;
} i2c_device_handler_t;

static i2c_device_handler_t DEVICE_I2C_DEFAULT = {
    .pin_sda = PICO_DEFAULT_I2C_SDA_PIN,
    .pin_scl = PICO_DEFAULT_I2C_SCL_PIN,
    .i2c_mod = PICO_DEFAULT_I2C,
    .fi2c_khz = 100,
    .init_done = false
};


/*! \brief Function for configuring the I2C interface of RP2040
* \param handler        Pointer to struct for setting-up the I2C interface module
* \return   Bool if configuration of I2C module was successful
*/
bool configure_i2c_module(i2c_device_handler_t *handler);


/*! \brief Function for checking if used addr on I2C bus is reserved (Fix 0x78 is ignored)
* \param addr   7bit address of device
*/
bool reserved_addr(uint8_t addr);


/*! \brief Function for scanning the I2C bus if devices are available
* \param handler        Pointer to struct of I2C module
*/
void scan_i2c_bus_for_device(i2c_device_handler_t *handler);

#endif