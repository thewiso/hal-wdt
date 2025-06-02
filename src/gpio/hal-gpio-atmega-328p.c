#include "hal-gpio.h"
#include "../../include/hal-gpio-atmega-328p.h"
#include <stddef.h>

#define PIN_PORT(pin_id)  ((pin_id) >> 8)  // Extracts the port from a pin id 
#define PIN_BIT(pin_id)   ((pin_id) & 0xFF) // Extracts the pin number from an pin id
#define BIT_MASK(pin) (1 << PIN_BIT(pin))

#define SET_BIT(register, pin_id) register |= BIT_MASK(pin_id)
#define CLEAR_BIT(register, pin_id) register &= ~BIT_MASK(pin_id)
#define READ_BIT(register, pin_id) (register & BIT_MASK(pin_id)) >> PIN_BIT(pin_id)

#define SFR_OFFSET 0x20
#define PIN_REGISTER(pin_id) (*(volatile uint8_t *)(PIN_PORT(pin_id) * 3 + 0x03 + SFR_OFFSET)) 
#define DDR_REGISTER(pin_id) (*(volatile uint8_t *)(PIN_PORT(pin_id) * 3 + 0x04 + SFR_OFFSET))
#define PORT_REGISTER(pin_id) (*(volatile uint8_t *)(PIN_PORT(pin_id) * 3 + 0x05 + SFR_OFFSET))

#define EIMSK (*(volatile uint8_t *)(0x1D + SFR_OFFSET))
#define INT0 0
#define INT1 1

#define EICRA (*(volatile uint8_t *)(0x69))
#define ISC00 0
#define ISC01 1
#define ISC10 2
#define ISC11 3

void set_high(uint32_t pin)
{
	SET_BIT(PORT_REGISTER(pin), pin);
}

void set_low(uint32_t pin)
{
	CLEAR_BIT(PORT_REGISTER(pin), pin);
}

void toggle(uint32_t pin){
	SET_BIT(PIN_REGISTER(pin), pin);
}

short read_digital(uint32_t pin){
	return READ_BIT(PORT_REGISTER(pin), pin);
}

void set_pin_mode(uint32_t pin, pin_mode pin_mode)
{
	switch (pin_mode)
	{
	case output:
		SET_BIT(DDR_REGISTER(pin), pin);
		break;
	case input:
		CLEAR_BIT(DDR_REGISTER(pin), pin);
		break;
	case input_pullup:
		CLEAR_BIT(DDR_REGISTER(pin), pin);
		SET_BIT(PORT_REGISTER(pin), pin);
	}
}

pin_mode get_pin_mode(uint32_t pin){
	if((READ_BIT(DDR_REGISTER(pin), pin)) == 1){
		return output;
	}else if((READ_BIT(PORT_REGISTER(pin), pin)) == 1){
		return input_pullup;
	}else{
		return input;
	}
}

void (*isr_int0)() = NULL;
void (*isr_int1)() = NULL;

// INT0_vect
void __vector_1(void) __attribute__((signal, used)); 
void __vector_1(void)
{
	if (isr_int0 != NULL)
	{
		isr_int0();
	}
}

// INT1_vect
void __vector_2(void) __attribute__((signal, used));
void __vector_2(void) {
    if (isr_int1 != NULL)
	{
		isr_int1();
	}
}

void set_interrupt_sense_control(volatile uint8_t *register_ptr, pin_interrupt_sense_mode sense_mode, short ISCX0_bit, short ISCX1_bit)
{
	switch (sense_mode)
	{
	case low_level:
		*register_ptr &= ~BIT_MASK(ISCX1_bit);
		*register_ptr &= ~BIT_MASK(ISCX0_bit);
		break;

	case any_change:
		*register_ptr &= ~BIT_MASK(ISCX1_bit);
		*register_ptr |= BIT_MASK(ISCX0_bit);
		break;

	case falling_edge:
		*register_ptr |= BIT_MASK(ISCX1_bit);
		*register_ptr &= ~BIT_MASK(ISCX0_bit);
		break;

	case rising_edge:
		*register_ptr |= BIT_MASK(ISCX1_bit);
		*register_ptr |= BIT_MASK(ISCX0_bit);
		break;
	}
}

/**
 * @brief ACHTUNG: Momentan nur Interrupts INT0 and INT1 unterstützt!
 * 
 * @param pin 
 * @param sense_mode 
 * @param isr 
 */
void setup_interrupt(uint32_t pin, pin_interrupt_sense_mode sense_mode, void (*isr)())
{
	short pin_bit = PIN_BIT(pin);
	short ISCX0_bit;
	short ISCX1_bit;

	switch (pin_bit)
	{
	case 2: // INT 0
		SET_BIT(EIMSK, INT0);
		isr_int0 = isr;
		ISCX0_bit = ISC00;
		ISCX1_bit = ISC01;
		break;

	case 3: // INT 1
		SET_BIT(EIMSK, INT1);
		isr_int1 = isr;
		ISCX0_bit = ISC10;
		ISCX1_bit = ISC11;
		break;

	default:
		return;
	}

	set_interrupt_sense_control(&EICRA, sense_mode, ISCX0_bit, ISCX1_bit);
	__asm__("sei"); // enable interrupts
}
