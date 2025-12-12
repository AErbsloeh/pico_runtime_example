#ifndef BTSTACK_SPP_H_
#define BTSTACK_SPP_H_


#include "hal/bt_spp/btstack_config.h"


/*! \brief  Function for initalising the BTstack for enabling the Bluetooth Classic SPP Procotol
* \return   Boolean if module is initialized
*/
bool btstack_spp_init(void);


/*! \brief  Function for starting the BTstack communication
* \return   Boolean if module is started
*/
bool btstack_spp_start(void);


#endif 
