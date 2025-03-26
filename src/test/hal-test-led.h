#pragma once

#include "hal-test.h"

struct TestDevice
{
	int green_led_pin; // TODO: unsigned
	int yellow_led_pin;
	int red_led_pin;
	int button_pin;
};

