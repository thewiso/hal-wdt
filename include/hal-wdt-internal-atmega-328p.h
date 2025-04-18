#pragma once

#include "hal-wdt.h"

struct WDTDevice 
{
};

enum WDTTimeout {
	ms16,
	ms32,
	ms64,
	ms125,
	ms250,
	ms500,
	ms1000,
	ms2000,
	ms4000,
	ms8000
};
