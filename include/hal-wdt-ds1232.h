#pragma once

#include "hal-wdt.h"
#include <stdint.h>

struct WDTDevice
{
	uint32_t enable_rst_pin; //Pin, der die Verbindung zwischen den RST Pin von DS1232 und Microcontroller aktiviert
	uint32_t td_pin; //Pin, der mit TD des DS1232 verbunden ist
	uint32_t not_st_pin; //Pin der mit not_ST des DS1232 verbunden ist
};

enum WDTTimeout {
	ms150,
	ms600,
	ms1200
};
