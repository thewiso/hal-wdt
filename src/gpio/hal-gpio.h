#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Mögliche Pin-Modi
 * 
 */
typedef enum {
	input,
	input_pullup,
	output
} pin_mode;

/**
 * @brief Mögliche Modi für Pin Interrupts
 * 
 */
typedef enum {
	low_level,
	any_change,
	falling_edge,
	rising_edge
} pin_interrupt_sense_mode;

/**
 * @brief Setzt logischen hohen Pegel am Pin
 * 
 * @param pin Als output konfigurierter Pin
 */
void set_high(uint32_t pin);

/**
 * @brief Setzt logischen niedrigen Pegel am Pin
 * 
 * @param pin Als output konfigurierter Pin
 */
void set_low(uint32_t pin);

/**
 * @brief Wechselt den gerade anliegenden logischen Pegel von hoch zu niedrig und andersherum
 * 
 * @param pin Als output konfigurierter Pin
 */
void toggle(uint32_t pin);

/**
 * @brief Gibt den gerade anliegenden logischen Pegel zurück
 * 
 * @param pin 
 * @return short Der anliegende logische Pegel [0, 1]
 */
short read_digital(uint32_t pin);

/**
 * @brief Konfiguriert den Pin als angegebener Modus
 * 
 * @param pin 
 * @param pin_mode 
 */
void set_pin_mode(uint32_t pin, pin_mode pin_mode);

/**
 * @brief Gibt den Modus den Pins zurück
 * 
 * @param pin 
 * @return pin_mode 
 */
pin_mode get_pin_mode(uint32_t pin);

/**
 * @brief Setzt einen Interrupt mit dem angegebenen Pin
 * 
 * @param pin 
 * @param sense_mode 
 * @param isr ISR, die bei Interrupt aufgerufen werden soll
 */
void setup_interrupt(uint32_t pin, pin_interrupt_sense_mode sense_mode, void (*isr)());