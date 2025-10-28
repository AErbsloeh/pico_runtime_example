#ifndef INIT_SYSTEM_H_
#define INIT_SYSTEM_H_


#include "pico/stdlib.h"
#include "stdbool.h"

/*! \brief Enum struct for defining suitable system state
*/
typedef enum{
    STATE_NONE,
    STATE_INIT,
    STATE_IDLE,
    STATE_ERROR
} system_state_t;


/*! \brief  Function for initialisating the GPIO and modules of RP Pico (used in this system)
*   \return                           Boolean for initialisation of RPi Pico device completed
*/
bool init_gpio_pico(void);


/*! \brief  Function for initialisating the System Functionalities
    \return                           Boolean for done initialisation of system
*/
bool init_system(void);


/*! \brief Function for getting the runtime since last reboot
* \return                           Time structure with runtime in ms
*/
uint64_t  get_runtime_ms(void);


/*! \brief Function for getting the status register of the system
*/
system_state_t get_system_state(void);


/*! \brief Function for getting the status register of the system
* \param state      New state to set
* \return           Boolean if new state is valid
*/
bool set_system_state(system_state_t state);


/*! \brief Function for getting the status register of the system
*/
uint64_t get_status_register(void);


#endif
