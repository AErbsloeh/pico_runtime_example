#ifndef SPI_HANDLER_H
#define SPI_HANDLER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"


/*! \brief Struct handler for configuring the SPI interface of RP2040
* \param pin_cs             GPIO num of used CS
* \param pin_mosi           GPIO num of used MOSI
* \param pin_sclk           GPIO num of used SCLK
* \param pin_miso           GPIO num of used MISO
* \param spi_mod            SPI handler of RP2040 (spi0 or spi1)
* \param fspi_khz           Used SPI clock [in kHz]
* \param mode               Used SPI mode
* \param msb_first          Send MSB first in transmission
* \param bits_per_transfer  Bit length in each SPI transmission
*/
typedef struct{
    uint8_t pin_cs;
    uint8_t pin_mosi;
    uint8_t pin_sclk;
    uint8_t pin_miso;
    spi_inst_t *spi_mod;
    uint16_t fspi_khz;
    uint8_t mode;
    bool msb_first;
    uint8_t bits_per_transfer;
    bool init_done;
} spi_device_handler_t;

static spi_device_handler_t DEVICE_SPI_DEFAULT = {
    .pin_cs = PICO_DEFAULT_SPI_CSN_PIN,
    .pin_mosi = PICO_DEFAULT_SPI_TX_PIN,
    .pin_sclk = PICO_DEFAULT_SPI_SCK_PIN,
    .pin_miso = PICO_DEFAULT_SPI_RX_PIN,
    .spi_mod = PICO_DEFAULT_SPI,
    .fspi_khz = 1000,
    .mode = 0,
    .msb_first = true,
    .bits_per_transfer = 16,
    .init_done = false
};


/*! \brief Function for configuring the SPI interface of RP2040
* \param handler        Pointer to struct for setting-up the SPI interface module
* \param use_spi_slave  Boolean if module is used as slave
* \return   Bool if configuration of SPI module was successful
*/
bool configure_spi_module(spi_device_handler_t *handler, bool use_spi_slave);


/*! \brief Function for configuring a software-defined SPI interface for RP2040
* \param handler        Pointer to struct for setting-up the SPI interface module
* \return   Bool if configuration of SPI module was successful
*/
bool configure_spi_module_soft(spi_device_handler_t *handler);


/*! \brief Function for sending data via software-defined SPI interface
* \param handler        Pointer to struct for setting-up the SPI interface module
* \return   Bool if configuration of SPI module was successful
*/
uint16_t send_data_spi_module_soft(spi_device_handler_t *handler, uint16_t data);


#endif
