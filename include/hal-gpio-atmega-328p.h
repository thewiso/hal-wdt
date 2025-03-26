#pragma once

#include "hal-gpio.h"

//Those calculations are preprocessed: https://en.wikipedia.org/wiki/C_preprocessor#Token_stringification
#define PLATFORM ATMEGA_328P

#define SFR_OFFSET 0x20

#define PORT_B 0
#define PB0 PIN_DEF(PORT_B, 0)
#define PB1 PIN_DEF(PORT_B, 1)
#define PB2 PIN_DEF(PORT_B, 2)
#define PB3 PIN_DEF(PORT_B, 3)
#define PB4 PIN_DEF(PORT_B, 4)
#define PB5 PIN_DEF(PORT_B, 5)
#define PB6 PIN_DEF(PORT_B, 6)
#define PB7 PIN_DEF(PORT_B, 7)

#define PORT_C 1
#define PC0 PIN_DEF(PORT_C, 0)
#define PC1 PIN_DEF(PORT_C, 1)
#define PC2 PIN_DEF(PORT_C, 2)
#define PC3 PIN_DEF(PORT_C, 3)
#define PC4 PIN_DEF(PORT_C, 4)
#define PC5 PIN_DEF(PORT_C, 5)
#define PC6 PIN_DEF(PORT_C, 6)

#define PORT_D 2
#define PD0 PIN_DEF(PORT_D, 0)
#define PD1 PIN_DEF(PORT_D, 1)
#define PD2 PIN_DEF(PORT_D, 2)
#define PD3 PIN_DEF(PORT_D, 3)
#define PD4 PIN_DEF(PORT_D, 4)
#define PD5 PIN_DEF(PORT_D, 5)
#define PD6 PIN_DEF(PORT_D, 6)
#define PD7 PIN_DEF(PORT_D, 7)

#define PIN_DEF(port, pin)  (((port) << 8) | (pin))
#define PIN_PORT(pin_def)  ((pin_def) >> 8)  // Extracts the port
#define PIN_BIT(pin_def)   ((pin_def) & 0xFF) // Extracts the pin number
#define BIT_MASK(pin) (1 << PIN_BIT(pin))

#define SET_BIT(register, pin_def) register |= BIT_MASK(pin_def)
#define CLEAR_BIT(register, pin_def) register &= ~BIT_MASK(pin_def)

// PINB 0x03
// DDRB 0x04
// PORTB 0x05

// PINC 0x06
// DDRC 0x07
// PORTC 0x08

// PIND 0x09
// DDRD 0x0A
// PORTD 0x0B
#define PIN_REGISTER(pin_def) (*(volatile char *)(PIN_PORT(pin_def) * 3 + 0x03 + SFR_OFFSET)) //TODO: check char datatype (everywhere!)
#define DDR_REGISTER(pin_def) (*(volatile char *)(PIN_PORT(pin_def) * 3 + 0x04 + SFR_OFFSET))
#define PORT_REGISTER(pin_def) (*(volatile char *)(PIN_PORT(pin_def) * 3 + 0x05 + SFR_OFFSET))

#define EIMSK (*(volatile char *)(0x1D + SFR_OFFSET))
#define INT0 0
#define INT1 1

#define EICRA (*(volatile char *)(0x69))
#define ISC00 0
#define ISC01 1
#define ISC10 2
#define ISC11 3