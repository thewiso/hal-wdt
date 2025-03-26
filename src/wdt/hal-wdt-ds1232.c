// #include "../include/hal-wdt.h"
// #include "../include/hal-wdt-ds1232.h"

// #define SFR_OFFSET 0x20

// /* ----------------------------------
// Watchdog Timer Control Register
// -------------------------------------*/
// #define WDTCSR (*(volatile char *)(0x60)) // Watchdog Timer Control Register

// #define WDT_PRESCALER_MASK 0x27 // 00100111

// #define WDT_INTERRUPT_MASK 0xC0 // 11000000

// /*
// This bit is used in timed sequences for changing WDE and prescaler bits. To clear the WDE bit, and/or change the prescaler
// bits, WDCE must be set.
// Once written to one, hardware will clear WDCE after four clock cycles.
// */
// #define WDCE 4 // Watchdog Change Enable

// /*
// WDE is overridden by WDRF in MCUSR. This means that WDE is always set when WDRF is set. To clear WDE, WDRF
// must be cleared first. This feature ensures multiple resets during conditions causing failure, and a safe start-up after the
// failure.
// */
// #define WDE 3 // Watchdog System Reset Enable

// /* ----------------------------------
// MCU Status Register
// -------------------------------------*/
// #define MCUSR (*(volatile char *)(0x35 + SFR_OFFSET)) // MCU Status Register

// /*
// This bit is set if a watchdog system reset occurs. The bit is reset by a power-on reset, or by writing a logic zero to the flag.
// */
// #define WDRF 3 // Watchdog System Reset Flag

// // TODO: make sure only one impl is included!


// void wdt_ds1232_set_pin(int pin){
	
// }

// bool wdt_is_timeout_valid(unsigned long timeout_ms)
// {
// 	switch (timeout_ms)
// 	{
// 	case 16:
// 	case 32:
// 	case 64:
// 	case 125:
// 	case 250:
// 	case 500:
// 	case 1000:
// 	case 2000:
// 	case 4000:
// 	case 8000:
// 		return true;
// 	}
// 	return false;
// }

// bool wdt_is_running() // this impl differs from the IC, should be noted in assignment!
// {
// 	// WDRF overrides WDE
// 	return (MCUSR >> WDRF) & 1 || (WDTCSR >> WDE) & 1;
// }

// // TODO check datatype, use uint32_t?
// unsigned long wdt_get_timeout_ms()
// {
// 	//the content of WDTSCR is:  WDIF WDIE WDP3 WDCE WDE WDP2 WDP1 WDP0 
// 	//we could shift values to our prescale_select looks like 0 0 0 0 WDP3 WDP2 WDP1 WDP0, so the values in our switch statement look more consistent
// 	//but as timeouts are an approximation from the documentation and cannot be calculated from the prescale and are therefore arbitrary,
// 	//we check the values how they appear in the masked register, simply because this is the fastest way
// 	char prescale_select = WDTCSR & WDT_PRESCALER_MASK;
// 	switch (prescale_select)
// 	{
// 	case 0x00:
// 		return 16;
// 	case 0x01:
// 		return 32;
// 	case 0x02:
// 		return 64;
// 	case 0x03:
// 		return 125;
// 	case 0x04:
// 		return 250;
// 	case 0x05:
// 		return 500;
// 	case 0x06:
// 		return 1000;
// 	case 0x07:
// 		return 2000;
// 	case 0x20:
// 		return 4000;
// 	case 0x21:
// 		return 8000;
// 	default:
// 		return -1;
// 	}
// }

// void wdt_start()
// {
// 	WDTCSR |= (1 << WDE);
// }

// void wdt_stop()
// {
// 	/*
// 	To clear WDE, WDRF must be cleared first

// 	The sequence for clearing WDE and changing time-out configuration is as follows:
// 		1. In the same operation, write a logic one to the watchdog change enable bit (WDCE) and WDE. A logic one must
// 		be written to WDE regardless of the previous value of the WDE bit.
// 		2. Within the next four clock cycles, write the WDE and watchdog prescaler bits (WDP) as desired, but with the
// 		WDCE bit cleared. This must be done in one operation.
// 	*/

// 	//only clearing the WDCE and WDE bits in WDTCSR after step 1 does not work, we have to write the whole register
// 	//so we save the current register without WDCE and WDE in a variable and use this in step 2
// 	unsigned char  temp_wdtcsr = WDTCSR & ~((1 << WDCE) | (1 << WDE));
// 	MCUSR &= ~(1 << WDRF);
// 	WDTCSR |= (1 << WDCE) | (1 << WDE);
// 	WDTCSR = temp_wdtcsr;
// }

// void wdt_kick()
// {
// 	__asm__("wdr");
// }

// bool wdt_setup(unsigned long timeout_ms, bool start)
// {
// 	// config->pin
// 	/*
// 	The sequence for clearing WDE and changing time-out configuration is as follows:
// 		1. In the same operation, write a logic one to the watchdog change enable bit (WDCE) and WDE. A logic one must
// 		be written to WDE regardless of the previous value of the WDE bit.
// 		2. Within the next four clock cycles, write the WDE and watchdog prescaler bits (WDP) as desired, but with the
// 		WDCE bit cleared. This must be done in one operation.
// 	*/
	
// 	unsigned char new_wdtcsr;
// 	switch (timeout_ms)
// 	{
// 	case 16:
// 		new_wdtcsr = 0x00;
// 		break;
// 	case 32:
// 		new_wdtcsr = 0x01;
// 		break;
// 	case 64:
// 		new_wdtcsr = 0x02;
// 		break;
// 	case 125:
// 		new_wdtcsr = 0x03;
// 		break;
// 	case 250:
// 		new_wdtcsr = 0x04;
// 		break;
// 	case 500:
// 		new_wdtcsr = 0x05;
// 		break;
// 	case 1000:
// 		new_wdtcsr = 0x06;
// 		break;
// 	case 2000:
// 		new_wdtcsr = 0x07;
// 		break;
// 	case 4000:
// 		new_wdtcsr = 0x20;
// 		break;
// 	case 8000:
// 		new_wdtcsr = 0x21;
// 		break;
// 	default:
// 		return false;
// 	}
	
// 	if(start){
// 		new_wdtcsr |= (1 << WDE);
// 	}
	
// 	// wir wollen WDIF und WDIE nicht ändern
// 	new_wdtcsr |= (WDTCSR & WDT_INTERRUPT_MASK);
	
// 	WDTCSR |= (1 << WDCE) | (1 << WDE);
// 	WDTCSR = new_wdtcsr;
// 	return true;
// }
