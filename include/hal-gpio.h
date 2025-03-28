#pragma once

#include <stdbool.h>

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

// void setup(int pin, bool mode);

void set_high(int pin);

void set_low(int pin);

void toggle(int pin);

short read_digital(int pin);

void set_pin_mode(int pin, pin_mode pin_mode);

pin_mode get_pin_mode(int pin);

void setup_interrupt(int pin, pin_interrupt_sense_mode sense_mode, void (*isr)());