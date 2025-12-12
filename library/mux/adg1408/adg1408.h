#ifndef ADG1408_H_
#define ADG1408_H_


#include "pico/stdlib.h"


// More informations on: https://www.analog.com/media/en/technical-documentation/data-sheets/adg1408_1409.pdf
// ================================= DEFINITIONS =================================
/*! \brief Struct handler for configuring the multiplexer ADG1408 from Analog Devices
* \param gpio_num_en    GPIO Pin for Enabling the Multiplexer
* \param gpio_num_a0    GPIO Pin for Address Line 0
* \param gpio_num_a1    GPIO Pin for Address Line 1
* \param gpio_num_a2    GPIO Pin for Address Line 2
* \param init_done      Boolean if device configuration is done        
*/
typedef struct{
    uint8_t gpio_num_en;
    uint8_t gpio_num_a0;
    uint8_t gpio_num_a1;
    uint8_t gpio_num_a2;
    bool init_done;
} adg1408_t;


static adg1408_t DEFAULT_ADG1408_CONFIG = {
    .gpio_num_en = 0,
    .gpio_num_a0 = 1,
    .gpio_num_a1 = 2,
    .gpio_num_a2 = 3,
    .init_done = false
};


// ================================= FUNCTIONS =================================
/*! \brief Function for configuring the multiplexer ADG1408 from Analog Devices
* \param handler    Device handler for using device
* \return           Boolean if initialization is done completely
*/
bool adg1408_init(adg1408_t *handler);


/*! \brief Function for enabling a channel of multiplexer ADG1408 from Analog Devices
* \param handler    Device handler for using device
* \param sel_chnnl  Value for selecting the number of channel (0 = disable, 1-8: channel)
*/
void adg1408_select_chnnl(adg1408_t *handler, uint8_t sel_chnnl);


#endif
