#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	input,
	input_pullup,
	output
} pin_mode;

typedef enum {
	low_level,
	any_change,
	falling_edge,
	rising_edge
} pin_interrupt_sense_mode;

void set_high(uint32_t pin);

void set_low(uint32_t pin);

void toggle(uint32_t pin);

short read_digital(uint32_t pin);

void set_pin_mode(uint32_t pin, pin_mode pin_mode);

pin_mode get_pin_mode(uint32_t pin);

void setup_interrupt(uint32_t pin, pin_interrupt_sense_mode sense_mode, void (*isr)());