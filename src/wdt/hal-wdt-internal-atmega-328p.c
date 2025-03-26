#include "../../include/hal-wdt-internal-atmega-328p.h"
#include <stddef.h>

bool wdt_is_timeout_valid(const WDTDevice_t* device, unsigned long timeout_ms) //TODO: check if Arduino uses the same clk frequency as in the ATmega doc
{
	switch (timeout_ms)
	{
	case 16:
	case 32:
	case 64:
	case 125:
	case 250:
	case 500:
	case 1000:
	case 2000:
	case 4000:
	case 8000:
		return true;
	}
	return false;
}

bool wdt_is_running(const WDTDevice_t* device) // this impl differs from the IC, should be noted in assignment!
{
	// WDRF overrides WDE
	return (MCUSR >> WDRF) & 1 || (WDTCSR >> WDE) & 1;
}

// TODO check datatype, use uint32_t?
unsigned long wdt_get_timeout_ms(const WDTDevice_t* device)
{
	//the content of WDTSCR is:  WDIF WDIE WDP3 WDCE WDE WDP2 WDP1 WDP0 
	//we could shift values to our prescale_select looks like 0 0 0 0 WDP3 WDP2 WDP1 WDP0, so the values in our switch statement look more consistent
	//but as timeouts are an approximation from the documentation and cannot be calculated from the prescale and are therefore arbitrary,
	//we check the values how they appear in the masked register, simply because this is the fastest way
	char prescale_select = WDTCSR & WDT_PRESCALER_MASK;
	switch (prescale_select)
	{
	case 0x00:
		return 16;
	case 0x01:
		return 32;
	case 0x02:
		return 64;
	case 0x03:
		return 125;
	case 0x04:
		return 250;
	case 0x05:
		return 500;
	case 0x06:
		return 1000;
	case 0x07:
		return 2000;
	case 0x20:
		return 4000;
	case 0x21:
		return 8000;
	default:
		return -1;
	}
}

void wdt_start(const WDTDevice_t* device)
{
	WDTCSR |= (1 << WDE);
}

void wdt_stop(const WDTDevice_t* device)
{
	/*
	To clear WDE, WDRF must be cleared first

	The sequence for clearing WDE and changing time-out configuration is as follows:
		1. In the same operation, write a logic one to the watchdog change enable bit (WDCE) and WDE. A logic one must
		be written to WDE regardless of the previous value of the WDE bit.
		2. Within the next four clock cycles, write the WDE and watchdog prescaler bits (WDP) as desired, but with the
		WDCE bit cleared. This must be done in one operation.
	*/

	//only clearing the WDCE and WDE bits in WDTCSR after step 1 does not work, we have to write the whole register
	//so we save the current register without WDCE and WDE in a variable and use this in step 2
	unsigned char  temp_wdtcsr = WDTCSR & ~((1 << WDCE) | (1 << WDE));
	MCUSR &= ~(1 << WDRF);
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = temp_wdtcsr;
}

void wdt_kick(const WDTDevice_t* device)
{
	__asm__("wdr");
}

bool wdt_setup(const WDTDevice_t* device, unsigned long timeout_ms, bool start)
{
	/*
	The sequence for clearing WDE and changing time-out configuration is as follows:
		1. In the same operation, write a logic one to the watchdog change enable bit (WDCE) and WDE. A logic one must
		be written to WDE regardless of the previous value of the WDE bit.
		2. Within the next four clock cycles, write the WDE and watchdog prescaler bits (WDP) as desired, but with the
		WDCE bit cleared. This must be done in one operation.
	*/
	
	unsigned char new_wdtcsr;
	switch (timeout_ms)
	{
	case 16:
		new_wdtcsr = 0x00;
		break;
	case 32:
		new_wdtcsr = 0x01;
		break;
	case 64:
		new_wdtcsr = 0x02;
		break;
	case 125:
		new_wdtcsr = 0x03;
		break;
	case 250:
		new_wdtcsr = 0x04;
		break;
	case 500:
		new_wdtcsr = 0x05;
		break;
	case 1000:
		new_wdtcsr = 0x06;
		break;
	case 2000:
		new_wdtcsr = 0x07;
		break;
	case 4000:
		new_wdtcsr = 0x20;
		break;
	case 8000:
		new_wdtcsr = 0x21;
		break;
	default:
		return false;
	}
	
	if(start){
		new_wdtcsr |= (1 << WDE);
	}
	
	// wir wollen WDIF und WDIE nicht ändern
	new_wdtcsr |= (WDTCSR & WDT_INTERRUPT_MASK);
	
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = new_wdtcsr;
	return true;
}

WDTDevice_t* wdt_create_device(){
	return NULL;
}