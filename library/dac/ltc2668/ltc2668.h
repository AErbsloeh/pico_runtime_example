#ifndef LTC2668_H_
#define LTC2668_H_


#include <stdio.h>
#include "hal/spi/spi.h"


// More informations on: https://www.analog.com/media/en/technical-documentation/data-sheets/2668fa.pdf
// ================================= DEFINITIONS =================================
#define LTC2668_RANGE_UNI_2VREF 0x00
#define LTC2668_RANGE_UNI_4VREF 0x01
#define LTC2668_RANGE_BI_2VREF  0x02
#define LTC2668_RANGE_BI_4VREF  0x03
#define LTC2668_RANGE_BI_VREF   0x04

#define LTC2668_ADR_DAC0    0x00 
#define LTC2668_ADR_DAC1    0x01 
#define LTC2668_ADR_DAC2    0x02 
#define LTC2668_ADR_DAC3    0x03 
#define LTC2668_ADR_DAC4    0x04 
#define LTC2668_ADR_DAC5    0x05 
#define LTC2668_ADR_DAC6    0x06 
#define LTC2668_ADR_DAC7    0x07 
#define LTC2668_ADR_DAC8    0x08 
#define LTC2668_ADR_DAC9    0x09 
#define LTC2668_ADR_DAC10   0x0A 
#define LTC2668_ADR_DAC11   0x0B 
#define LTC2668_ADR_DAC12   0x0C 
#define LTC2668_ADR_DAC13   0x0D 
#define LTC2668_ADR_DAC14   0x0E 
#define LTC2668_ADR_DAC15   0x0F

#define LTC2668_PWRUP_DAC0      0x0001
#define LTC2668_PWRUP_DAC1      0x0002
#define LTC2668_PWRUP_DAC2      0x0004
#define LTC2668_PWRUP_DAC3      0x0008
#define LTC2668_PWRUP_DAC4      0x0010
#define LTC2668_PWRUP_DAC5      0x0020
#define LTC2668_PWRUP_DAC6      0x0040
#define LTC2668_PWRUP_DAC7      0x0080
#define LTC2668_PWRUP_DAC8      0x0100
#define LTC2668_PWRUP_DAC9      0x0200
#define LTC2668_PWRUP_DAC10     0x0400
#define LTC2668_PWRUP_DAC11     0x0800
#define LTC2668_PWRUP_DAC12     0x1000
#define LTC2668_PWRUP_DAC13     0x2000
#define LTC2668_RWRUP_DAC14     0x4000
#define LTC2668_PWRUP_DAC15     0x8000
#define LTC2668_PWRUP_DAC_ALL   0xFFFF


/*! \brief Struct handler for configuring the Digital-Analog Converter LTC2668 from Linear Technologies/Analog Devices
* \param spi_handler    Predefined SPI handler for RP2040
* \param gpio_num_csn   GPIO Pin for Chip Select
* \param gpio_num_clrn  GPIO Pin for Clear Line (optional)
* \param use_clrn_hw    Boolean if data clear line is used 
* \param pwr_up_chnnl   uint16_t value with power up specific DAC channel
* \param use_int_vref   Boolean for using the internal voltage reference source (V_ref,int = 2.5V)
* \param vref_range     Span code for setting the voltage range
* \param use_16bit_dev  Boolean if 16-bit version is used otherwise 12-bit
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    spi_t *spi_handler;
    uint8_t gpio_num_csn;
    uint8_t gpio_num_clrn;
    bool use_clrn_hw;
    uint16_t pwr_up_chnnl;
    bool use_int_vref;
    uint8_t vref_range;
    bool use_16bit_dev;
    bool init_done;
} ltc2668_t;


static ltc2668_t LTC2668_DEFAULT_CONFIG = {
    .spi_handler = &DEVICE_SPI_DEFAULT,
    .gpio_num_csn = 1,
    .gpio_num_clrn = 0,
    .use_clrn_hw = false,
    .pwr_up_chnnl = LTC2668_PWRUP_DAC_ALL,
    .use_int_vref = true,
    .vref_range = LTC2668_RANGE_BI_VREF,
    .use_16bit_dev = true,
    .init_done = false
};


// ================================= FUNCTIONS =================================
/*! \brief Function for configuring the the Digital-Analog-Converter LTC2668
* \param cnf  	Device handler for using device
* \return       Boolean if initialization is done completely
*/
bool ltc2668_init(ltc2668_t *cnf);


/*! \brief Function for clearing the output (soft) of the Digital-Analog-Converter LTC2668
* \param cnf  			Device handler for using device
* \param chnnl          Channel number for updating the voltage reference mode
* \param vref_mode      Reference mode (called with LTC2668_RANGE_<x>)
*/
void ltc2668_update_vrange(ltc2668_t *cnf, uint8_t chnnl, uint8_t vref_mode);


/*! \brief Function for clearing the output (soft) of the Digital-Analog-Converter LTC2668
* \param cnf  Device handler for using device
*/
void ltc2668_clear_data_soft(ltc2668_t *cnf);


/*! \brief Function for clearing the output of the Digital-Analog-Converter LTC2668
* \param cnf  Device handler for using device
*/
void ltc2668_clear_data(ltc2668_t *cnf);


/*! \brief Function for controlling the internal MUX of the DAC LTC2668
* \param cnf  Device handler for using device
* \param enable         Enable mux control
* \param channel        Selected channel number
*/
void ltc2668_mux_control(ltc2668_t *cnf, bool enable, uint8_t chnnl);


/*! \brief Function for writing the data output (and not update) of all channels of the DAC LTC2668
* \param cnf  Device handler for using device
* \param data           Data to apply on all chnnls
*/
void ltc2668_write_output_all_channel(ltc2668_t *cnf, uint16_t data);


/*! \brief Function for writing the data output (and not update) of one channel of the DAC LTC2668
* \param cnf  Device handler for using device
* \param data           Data to apply on all channels
* \param chnnl        	Selected channel to update the data
*/
void ltc2668_write_output_single_channel(ltc2668_t *cnf, uint16_t data, uint8_t chnnl);


/*! \brief Function for updating the data output of all channels of the DAC LTC2668
* \param cnf  Device handler for using device
* \param data           Data to apply on all channels
*/
void ltc2668_update_output_all_channel(ltc2668_t *cnf, uint16_t data);


/*! \brief Function for updating the data output of one channel of the DAC LTC2668
* \param cnf  Device handler for using device
* \param data           Data to apply on all channels
* \param chnnl        	Selected channel to update the data
*/
void ltc2668_update_output_single_channel(ltc2668_t *cnf, uint16_t data, uint8_t chnnl);


#endif
