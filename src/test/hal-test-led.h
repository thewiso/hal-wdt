#pragma once

#include "hal-test.h"
#include <stdint.h>

struct TestDevice
{
	uint32_t green_led_pin; 
	uint32_t yellow_led_pin;
	uint32_t red_led_pin;
	uint32_t button_pin;
};

