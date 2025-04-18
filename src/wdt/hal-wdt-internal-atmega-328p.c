#include "../../include/hal-wdt-internal-atmega-328p.h"
#include <stddef.h>
#include <stdint.h>

#define SFR_OFFSET 0x20

/* ----------------------------------
Watchdog Timer Control Register
-------------------------------------*/
#define WDTCSR (*(volatile uint8_t *)(0x60)) // Watchdog Timer Control Register

#define WDT_PRESCALER_MASK 0x27 // 00100111

#define WDT_INTERRUPT_MASK 0xC0 // 11000000

/*
This bit is used in timed sequences for changing WDE and prescaler bits. To clear the WDE bit, and/or change the prescaler
bits, WDCE must be set.
Once written to one, hardware will clear WDCE after four clock cycles.
*/
#define WDCE 4 // Watchdog Change Enable

/*
WDE is overridden by WDRF in MCUSR. This means that WDE is always set when WDRF is set. To clear WDE, WDRF
must be cleared first. This feature ensures multiple resets during conditions causing failure, and a safe start-up after the
failure.
*/
#define WDE 3 // Watchdog System Reset Enable

/* ----------------------------------
MCU Status Register
-------------------------------------*/
#define MCUSR (*(volatile uint8_t *)(0x35 + SFR_OFFSET)) // MCU Status Register

/*
This bit is set if a watchdog system reset occurs. The bit is reset by a power-on reset, or by writing a logic zero to the flag.
*/
#define WDRF 3 // Watchdog System Reset Flag

bool wdt_is_running(const WDTDevice_t* device) 
{
	// WDRF overrides WDE
	return (MCUSR >> WDRF) & 1 || (WDTCSR >> WDE) & 1;
}

WDTTimeout_t wdt_get_timeout(const WDTDevice_t* device)
{
	//the content of WDTSCR is:  WDIF WDIE WDP3 WDCE WDE WDP2 WDP1 WDP0 
	//we could shift values to our prescale_select looks like 0 0 0 0 WDP3 WDP2 WDP1 WDP0, so the values in our switch statement look more consistent
	//but as timeouts are an approximation from the documentation and cannot be calculated from the prescale and are therefore arbitrary,
	//we check the values how they appear in the masked register, simply because this is the fastest way
	uint8_t prescale_select = WDTCSR & WDT_PRESCALER_MASK;
	switch (prescale_select)
	{
	case 0x00:
		return ms16;
	case 0x01:
		return ms32;
	case 0x02:
		return ms64;
	case 0x03:
		return ms125;
	case 0x04:
		return ms250;
	case 0x05:
		return ms500;
	case 0x06:
		return ms1000;
	case 0x07:
		return ms2000;
	case 0x20:
		return ms4000;
	case 0x21:
		return ms8000;
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
	uint8_t temp_wdtcsr = WDTCSR & ~((1 << WDCE) | (1 << WDE));
	MCUSR &= ~(1 << WDRF);
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = temp_wdtcsr;
}

void wdt_kick(const WDTDevice_t* device)
{
	asm("wdr"); 
}

void wdt_setup(const WDTDevice_t* device, WDTTimeout_t timeout, bool start)
{
	/*
	The sequence for clearing WDE and changing time-out configuration is as follows:
		1. In the same operation, write a logic one to the watchdog change enable bit (WDCE) and WDE. A logic one must
		be written to WDE regardless of the previous value of the WDE bit.
		2. Within the next four clock cycles, write the WDE and watchdog prescaler bits (WDP) as desired, but with the
		WDCE bit cleared. This must be done in one operation.
	*/
	
	uint8_t new_wdtcsr;
	switch (timeout)
	{
	case ms16:
		new_wdtcsr = 0x00;
		break;
	case ms32:
		new_wdtcsr = 0x01;
		break;
	case ms64:
		new_wdtcsr = 0x02;
		break;
	case ms125:
		new_wdtcsr = 0x03;
		break;
	case ms250:
		new_wdtcsr = 0x04;
		break;
	case ms500:
		new_wdtcsr = 0x05;
		break;
	case ms1000:
		new_wdtcsr = 0x06;
		break;
	case ms2000:
		new_wdtcsr = 0x07;
		break;
	case ms4000:
		new_wdtcsr = 0x20;
		break;
	case ms8000:
		new_wdtcsr = 0x21;
		break;
	default:
		return;
	}
	
	if(start){
		new_wdtcsr |= (1 << WDE);
	}
	
	// wir wollen WDIF und WDIE nicht ändern
	new_wdtcsr |= (WDTCSR & WDT_INTERRUPT_MASK);
	
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = new_wdtcsr;
}

WDTDevice_t* wdt_create_device(){
	return NULL;
}