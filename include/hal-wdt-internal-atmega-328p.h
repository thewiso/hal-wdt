#include "hal-wdt.h"

#define SFR_OFFSET 0x20

/* ----------------------------------
Watchdog Timer Control Register
-------------------------------------*/
#define WDTCSR (*(volatile char *)(0x60)) // Watchdog Timer Control Register

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
#define MCUSR (*(volatile char *)(0x35 + SFR_OFFSET)) // MCU Status Register

/*
This bit is set if a watchdog system reset occurs. The bit is reset by a power-on reset, or by writing a logic zero to the flag.
*/
#define WDRF 3 // Watchdog System Reset Flag

struct WDTDevice
{
};
