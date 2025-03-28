#include "../../include/hal-gpio-atmega-328p.h"
#include <stddef.h>

void set_high(int pin)
{
	SET_BIT(PORT_REGISTER(pin), pin);
}

void set_low(int pin)
{
	CLEAR_BIT(PORT_REGISTER(pin), pin);
}

void toggle(int pin){
	SET_BIT(PIN_REGISTER(pin), pin);
}

short read_digital(int pin){
	return READ_BIT(PORT_REGISTER(pin), pin);
}

void set_pin_mode(int pin, pin_mode pin_mode)
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

pin_mode get_pin_mode(int pin){
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

void set_interrupt_sense_control(volatile char *register_ptr, pin_interrupt_sense_mode sense_mode, short ISCX0_bit, short ISCX1_bit)
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

// currently only INT0 and INT1 supported!
void setup_interrupt(int pin, pin_interrupt_sense_mode sense_mode, void (*isr)())
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

	// SET_BIT(EIMSK, INT1);  // aktivieren des INT1 interrupts
	// SET_BIT(EICRA, ISC11); // falling edge
	// sei(); // enable interrupts
}
