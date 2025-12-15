#ifndef PWR_WATCH_H_
#define PWR_WATCH_H_


#include "pico/stdlib.h"


// ============================= DECLARATIONS =============================
typedef struct {
    uint8_t pin_en;
    uint8_t pin_led;
    bool use_pgd;
    uint8_t pin_pgd;
    bool state;
    bool init_done;
} power_single_t;


typedef struct {
    uint8_t pin_en_reg;
    uint8_t pin_en_ldo;
    uint8_t pin_led;
    bool use_pgd;
    uint8_t pin_pgd;
    bool state;
    bool init_done;
} power_dual_t;


// ============================= HELP FUNCTIONS =============================
/*! \brief Function for monitoring the PowerGood GPIO for handling on device
 * \param pin_pgd    Pin number for monitoring the power down signal
 * \param led_state  State of the LED (true: ON, false: OFF)
 * \param config     Pointer to the power configuration struct for single power supply 
 * \return           Boolean if power down is detected
*/
void gpio_isr_pwr_monitor_single(uint gpio, uint32_t events, power_single_t *config);

/*! \brief Function for monitoring the PowerGood GPIO for handling on device
 * \param pin_pgd    Pin number for monitoring the power down signal
 * \param led_state  State of the LED (true: ON, false: OFF)
 * \param config     Pointer to the power configuration struct for dual power supply 
 * \return           Boolean if power down is detected
*/
void gpio_isr_pwr_monitor_double(uint gpio, uint32_t events, power_dual_t *config);

// ============================= FUNCTIONS FOR SINGLE POWER SUPPLY =============================
/*! \brief Function for initializing the single power supply
 * \param config    Pointer to the power configuration struct for single power supply 
 * \return          Boolean if initialization is done completely
*/
bool init_system_power_single(power_single_t *config);

/*! \brief Function for enabling the single power supply
 * \param config    Pointer to the power configuration struct for single power supply 
 * \return          Boolean if initialization is done completely
*/
bool enable_system_power_single(power_single_t *config);

/*! \brief Function for disabling the single power supply
 * \param config    Pointer to the power configuration struct for single power supply 
 * \return          Boolean if initialization is done completely
*/
bool disable_system_power_single(power_single_t *config);

// ============================= FUNCTIONS FOR DUAL POWER SUPPLY =============================
/*! \brief Function for initializing the dual power supply
 * \param config    Pointer to the power configuration struct for single power supply 
 * \return          Boolean if initialization is done completely
*/
bool init_system_power_dual(power_dual_t *config);

/*! \brief Function for enabling the dual power supply
 * \param config    Pointer to the power configuration struct for single power supply 
 * \return          Boolean if initialization is done completely
*/
bool enable_system_power_dual(power_dual_t *config);

/*! \brief Function for disabling the dual power supply
 * \param config    Pointer to the power configuration struct for single power supply 
 * \return          Boolean if initialization is done completely
*/
bool disable_system_power_dual(power_dual_t *config);


#endif
