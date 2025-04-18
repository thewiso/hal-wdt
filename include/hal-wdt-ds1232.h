#pragma once

#include "hal-wdt.h"
#include <stdint.h>

struct WDTDevice
{
	uint32_t enable_rst_pin; //used to provide DS1232 with power
	uint32_t td_pin; //used for timeout period 
	uint32_t not_st_pin; //used for kicking wdt
};

enum WDTTimeout {
	ms150,
	ms600,
	ms1200
};
