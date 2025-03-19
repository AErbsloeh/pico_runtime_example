#ifndef POWER_WATCH_H_
#define POWER_WATCH_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


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
void gpio_isr_pwr_monitoring(uint gpio, uint32_t events);
bool monitor_system_power_pgd_start(uint8_t pin_pgd);


// ============================= FUNCTIONS =============================
bool init_system_power_single(power_single_t *config);
bool enable_system_power_single(power_single_t *config);
bool disable_system_power_single(power_single_t *config);

bool init_system_power_dual(power_dual_t *config);
bool enable_system_power_dual(power_dual_t *config);
bool disable_system_power_dual(power_dual_t *config);


#endif
