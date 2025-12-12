#ifndef AD5141_SPI_H_
#define AD5141_SPI_H_


#include "hal/spi/spi.h"


// ========================================================== DEFINITIONS ==========================================================
/*! \brief Struct handler for configuring the Digital Potentiometer AD5141 from Analog Devices with SPI interface
* \param spi_handler    Predefined SPI handler for RP2040
* \param device_csn     GPIO Pin I2C device adresse (will be defined in init function)
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    spi_t *spi_handler;
    uint8_t gpio_csn;
    bool init_done;
} ad5141_spi_t;

static ad5141_spi_t DEVICE_AD5141_DEFAULT = {
    .spi_handler = &DEVICE_SPI_DEFAULT,
    .gpio_csn = 0,
    .init_done = false
};


// ========================================================== FUNCTIONS ==========================================================
/*! \brief Function for device initialization of Digital Potentiometer AD5141
*   \param gpio_csn    Predefined SPI device handler
*   \return                 bool with init_done
*/
bool ad5141_spi_init(ad5141_spi_t *gpio_csn);


/*! \brief Function for Software Reset of Digital Potentiometer AD5141
*   \param gpio_csn    Predefined SPI device handler
*/
bool ad5141_spi_reset_software(ad5141_spi_t *gpio_csn);


/*! \brief Function for Controling the Shutdown Register of Digital Potentiometer AD5141
*   \param gpio_csn    Predefined SPI device handler
*   \param enable_rdac0     Boolean for enabling the RDAC0
*   \param enable_rdac1     Boolean for enabling the RDAC1
*/
bool ad5141_spi_control_shutdown(ad5141_spi_t *gpio_csn, bool enable_rdac0, bool enable_rdac1);


/*! \brief Function for Defining the Wiper Position of selected Channel from Digital Potentiometer AD5141
*   \param gpio_csn    Predefined SPI device handler
*   \param rdac_sel         Number for selecting the wiper (RDAC0, RDAC1)
*   \param pot_position     Potentiometer Position (8-bit)
*/
bool ad5141_spi_define_level(ad5141_spi_t *gpio_csn, uint8_t rdac_sel, uint8_t pot_position);


#endif
