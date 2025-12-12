#ifndef AD57X4_H_
#define AD57X4_H_


#include "hal/spi/spi.h"


// More informations on: https://www.analog.com/media/en/technical-documentation/data-sheets/ad5724_5734_5754.pdf
// ================================= DEFINITIONS =================================
#define AD57x4_REG_DATA     0x00
#define AD57x4_REG_RANGE    0x01
#define AD57x4_REG_PWR      0x02
#define AD57x4_REG_CNTRL    0x03

#define AD57x4_ADR_DAC0     0x00
#define AD57x4_ADR_DAC1     0x01
#define AD57x4_ADR_DAC2     0x02
#define AD57x4_ADR_DAC3     0x03
#define AD57x4_ADR_DAC_ALL  0x04

#define AD57x4_PWR_DAC0     0x01
#define AD57x4_PWR_DAC1     0x02
#define AD57x4_PWR_DAC2     0x04
#define AD57x4_PWR_DAC3     0x08
#define AD57x4_PWR_DAC_All  0x0F

#define AD57x4_RANGE_UNI_5V     0x00
#define AD57x4_RANGE_UNI_10V    0x01
#define AD57x4_RANGE_UNI_10V8   0x02
#define AD57x4_RANGE_BI_5V      0x03
#define AD57x4_RANGE_BI_10V     0x04
#define AD57x4_RANGE_BI_10V8    0x05


// ================================= FUNCTIONS =================================
/*! \brief Struct for handling the configuration of the Digital-Analog Converter (DAC) AD57x4 from Analog Devices
* \param bitwidth   Used data bitwidth of device
*/
typedef struct {
    spi_t *spi_mod;
    uint8_t gpio_num_csn;
    bool use_gpio_ldac;
    uint8_t gpio_num_ldac;
    bool use_gpio_dclr;
    uint8_t gpio_num_dclr;
    uint8_t range_mode;
    uint8_t en_pwr_chnnl;
    uint8_t bitwidth;
    bool spi_sdo_disable;
    bool init_done;
} ad57x4_t;


static ad57x4_t DEVICE_AD57X4_DEFAULT = {
    .spi_mod = &DEVICE_SPI_DEFAULT,
    .gpio_num_csn = PICO_DEFAULT_SPI_CSN_PIN,
    .use_gpio_ldac = false,
    .gpio_num_ldac = 0,
    .use_gpio_dclr = false,
    .gpio_num_dclr = 0,
    .range_mode = AD57x4_RANGE_UNI_5V,
    .en_pwr_chnnl = AD57x4_PWR_DAC0 | AD57x4_PWR_DAC1 | AD57x4_PWR_DAC2 | AD57x4_PWR_DAC3,
    .spi_sdo_disable = true,
    .bitwidth = 16,
    .init_done = false
};


// ================================= FUNCTIONS =================================
/*! \brief Function for initialising the Digital-Analog Converter AD57x4 from Analog Devices
* \param config     Struct type for configurating the device
* \return           Boolean if initialization is done successful
*/
bool ad57x4_init(ad57x4_t *config);


/*! \brief Function for resetting the device AD57x4 on the device bus
* \param config     Struct type for configurating the device
* \return           Boolean if reset is done successful
*/
bool ad57x4_reset(ad57x4_t *config);


/*! \brief Function for updating the DAC channel output
* \param config     Struct type for configurating the device
* \param chnnl      Selected channel (0, 1, 2, 3, ALL) using definitions
* \param data       Unsigned integer data with new output value
* \return           Integer value with number of transmitted bytes
*/
int8_t ad57x4_update_data(ad57x4_t *config, uint8_t chnnl, uint16_t data);


#endif
