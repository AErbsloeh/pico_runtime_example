#ifndef HAL_PWM_H_
#define HAL_PWM_H_


#include <stdio.h>
#include "pico/stdlib.h"

/**
 * @file pwm.h
 * @brief PWM Hardware Abstraction Layer für Raspberry Pi Pico.
 *
 * Diese HAL abstrahiert die PWM-Konfiguration, Duty-Cycle,
 * Periodenänderungen und Interrupt-Steuerung im Stil der
 * Timer-HAL des Nutzers.
 */
/* ============================================================
 *                  DATENSTRUKTUREN
 * ============================================================ */

/**
 * @struct pwm_t
 * @brief Enthält alle Konfigurationsparameter und Statuswerte für ein PWM-Modul.
 * \param gpio            GPIO-Pin für die PWM-Ausgabe
 * \param slice           PWM-Slice-Nummer (wird automatisch gesetzt)
 * \param irq_number      IRQ-Nummer (typisch: PWM_IRQ_WRAP)
 * \param use_irq         true = Interrupts werden benutzt, false = kein IRQ
 * \param clk_div         Clock Divider für PWM Slice
 * \param wrap            PWM Wrap-Wert → bestimmt Frequenz
 * \param level           PWM Duty Cycle (0..wrap)
 * \param enable_state    Zustand: PWM aktiv oder deaktiviert
 * \param init_done       Flag, ob PWM initialisiert wurde
 * \param alarm_done      Flag für IRQ-Synchronisationsvorgänge
 */
typedef struct {
    uint8_t gpio;
    uint8_t slice;
    uint8_t irq_number;
    bool use_irq;
    uint16_t clk_div;
    uint16_t wrap;
    uint16_t level;
    bool enable_state;
    bool init_done;
    bool alarm_done;
} pwm_t;


/* ============================================================
 *                  FUNKTIONS-PROTOTYPEN
 * ============================================================ */
/**
 * @brief Initialisiert eine PWM-Instanz gemäß der übergebenen Handler-Konfiguration.
 *
 * Legt GPIO-Funktion, Slice, clk_div und wrap fest.
 * Optional wird ein IRQ eingerichet (wenn handler->use_irq = true).
 *
 * @param handler Pointer auf pwm_t Struktur
 * @return true bei erfolgreicher Initialisierung
 */
bool init_pwm_irq(pwm_t *handler);


/**
 * @brief Aktiviert die PWM-Ausgabe.
 *
 * Setzt den Duty-Cycle und aktiviert das entsprechende PWM-Slice.
 *
 * @param handler Pointer auf pwm_t
 * @return true wenn PWM erfolgreich aktiviert wurde
 */
bool enable_pwm(pwm_t *handler);


/**
 * @brief Deaktiviert die PWM-Ausgabe.
 *
 * @param handler Pointer auf pwm_t
 * @return true wenn PWM erfolgreich deaktiviert wurde
 */
bool disable_pwm(pwm_t *handler);


/**
 * @brief Setzt PWM-Level (Duty Cycle) direkt als 16-bit Wert.
 *
 * Wichtig: level muss <= wrap sein.
 *
 * @param handler Pointer auf pwm_t
 * @param level PWM Level (0..wrap)
 * @return true wenn erfolgreich
 */
bool pwm_set_level(pwm_t *handler, uint16_t level);


/**
 * @brief Aktualisiert den Duty-Cycle direkt als 16-bit Wert.
 * @param handler       Pointer auf pwm_t
 * @param duty_cycle    Duty Cycle (0..wrap)
 * @return              true wenn erfolgreich
 */
bool pwm_update_duty_cycle(pwm_t *handler, uint16_t duty_cycle);

#endif
