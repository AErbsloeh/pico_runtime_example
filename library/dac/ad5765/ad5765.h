#ifndef AD5765_H_
#define AD5765_H_


#include "hal/spi/spi.h"


// More informations on: https://www.analog.com/media/en/technical-documentation/data-sheets/AD5765.pdf
// =========================================== DEFINITIONS ==============================================
#define AD5765_ADR_DAC0     0x00
#define AD5765_ADR_DAC1     0x01
#define AD5765_ADR_DAC2     0x02
#define AD5765_ADR_DAC3     0x03
#define AD5765_ADR_DAC_ALL  0x04

#define AD5765_REG_FUNC         0x00
#define AD5765_REG_DATA         0x02
#define AD5765_REG_GAIN_COARSE  0x03
#define AD5765_REG_GAIN_FINE    0x04
#define AD5765_REG_OFFSET       0x05


/*! \brief Struct handler for configuring the Digital-Analog-Converter (DAC) AD5765 from Analog Devices with SPI interface
* \param spi_handler    SPI handler of RP2040
* \param gpio_num_csn   GPIO number of Chip Select Line
* \param use_gpio_rst   Boolean for selecting Hardware Reset functionality
* \param gpio_num_rst   GPIO number of Reset Line (Actice Low)
* \param use_gpio_ldac  Boolean for selecting Hardware LDAC functionality
* \param gpio_num_ldac  GPIO number for LDAC line (Active Low)
* \param use_gpio_clr   Boolean for selecting Hardware Clear functionality
* \param gpio_num_clr   GPIO number for CLR line (Active Low)
* \param state_gpio0    Ouptut state of GPIO D0 
* \param state_gpio1    Output state of GPIO D1
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    spi_t *spi_handler;
    uint8_t gpio_num_csn;
    bool use_gpio_rst;
    uint8_t gpio_num_rst;
    bool use_gpio_ldac;
    uint8_t gpio_num_ldac;
    bool use_gpio_clr;
    uint8_t gpio_num_clr;
    bool state_gpio0;
    bool state_gpio1;
    bool use_local_gnd_offset;
    bool init_done;
} ad5765_t;


static ad5765_t DEVICE_AD5765_DEFAULT = {
    .spi_handler = &DEVICE_SPI_DEFAULT,
    .gpio_num_csn = PICO_DEFAULT_SPI_CSN_PIN,
    .use_gpio_rst = false,
    .gpio_num_rst = 0,
    .gpio_num_ldac = false,
    .gpio_num_ldac = 0,
    .use_gpio_clr = false,
    .gpio_num_clr = 0,
    .state_gpio0 = false,
    .state_gpio1 = false,
    .use_local_gnd_offset = false,
    .init_done = false
};

// =========================================== FUNCTIONS ==============================================
/*! \brief Function for configuring the Digital-Analog-Converter (DAC) AD5765 from Analog Devices with SPI interface
* \param handler        Device handler 
* \return               Boolean if initialization is done
*/
bool ad5765_init(ad5765_t *handler);


/*! \brief Function for performing reset of the Digital-Analog-Converter (DAC) AD5765 from Analog Devices with SPI interface
* \param handler        Device handler 
* \return               Boolean if reset is done
*/
bool ad5765_reset(ad5765_t *handler);


/*! \brief Function for updating data on channel
* \param handler        Device handler 
* \param update_data    Boolean for updating the data after transmission
* \param chnnl          Selected adresse for updating DAC channel
* \param data           uint16_t value for updating the output
* \return               Number of transmitted bytes (-1= error)
*/
int8_t ad5765_update_data(ad5765_t *handler, bool update_data, uint8_t chnnl, uint16_t data);


/*! \brief Function for cleaering reset of the Digital-Analog-Converter (DAC) AD5765 from Analog Devices with SPI interface
* \param handler        Device handler 
* \return               Number of transmitted bytes (-1= error)
*/
int8_t ad5765_clear_data(ad5765_t *handler);


/*! \brief Function for performing reset of the Digital-Analog-Converter (DAC) AD5765 from Analog Devices with SPI interface
* \param handler        Device handler 
* \param sel_gpio       Selected GPIO output channel (false: 0, true: 1)
* \param state_gpio     Boolean for updating the GPIO output 
* \return               Number of transmitted bytes (-1= error)
*/
int8_t ad5765_update_gpio(ad5765_t *handler, bool sel_gpio, bool state_gpio);


#endif
