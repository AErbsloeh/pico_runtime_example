#ifndef BTSTACK_SPP_COUNTER_H_
#define BTSTACK_SPP_COUNTER_H_


/*! \brief  Function for initalising the BTstack for enabling the Bluetooth Classic SPP Procotol
* \return   Boolean if module is initialized
*/
bool btstack_spp_init(void);


/*! \brief  Function for starting the BTstack communication
* \return   Boolean if module is started
*/
bool btstack_spp_start(void);


#endif 
