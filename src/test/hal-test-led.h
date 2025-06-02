#pragma once

#include "hal-test.h"
#include <stdint.h>

struct TestDevice
{
	uint32_t green_led_pin; //Pin, der mit grüner LED verbunden ist
	uint32_t yellow_led_pin; //Pin, der mit gelber LED verbunden ist
	uint32_t red_led_pin; //Pin, der mit roter LED verbunden ist
	uint32_t button_pin; //Pin, der mit Taster verbunden ist
};

