#ifndef INIT_SYSTEM_H_
#define INIT_SYSTEM_H_


#include "hardware_io.h"


/*! \brief  Function for initialisating the GPIO and modules of RP Pico (used in this system)
*   \param wait_until_usb_connected   Boolean for waiting until USB (terminal opened) is connected
*   \return                           Boolean for initialisation of RPi Pico device completed
*/
bool init_gpio_pico(bool wait_until_usb_connected);


/*! \brief  Function for initialisating the System Functionalities
    \return                           Boolean for done initialisation of system
*/
bool init_system(void);


#endif