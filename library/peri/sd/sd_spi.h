#ifndef SD_SPI_H_
#define SD_SPI_H_


#include "hal/spi/spi.h"


/*! \brief Struct for configuring the module for SD card slot on board
* \param spi            Pointer to SPI interface
* \param gpio_cs        Chip Select GPIO pin
* \param card_available Boolen if card is available
* \param init_done      Initialization flag
*/
typedef struct {
    
    spi_t *spi;                 // Pointer to SPI interface
    uint8_t gpio_cs;            // Chip Select GPIO pin
    bool card_available;        // Card available flag    
    bool init_done;             // Initialization flag
} sd_spi_t;



bool sd_spi_init(sd_spi_t *config);



#endif
