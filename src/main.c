#include "../include/hal-wdt-internal-atmega-328p.h"
#include "../include/hal-gpio-atmega-328p.h"
#include "test/hal-test-led.h"
#include "test/automated-test.h"
#include "test/manual-test.h"
#include <stddef.h>

int main()
{
	TestDevice_t* test_device = test_create_device();

	test_device->green_led_pin = PD0;
	test_device->yellow_led_pin = PD1;
	test_device->red_led_pin = PD2;
	test_device->button_pin = PD3;

	WDTDevice_t* wdt_device = wdt_create_device();

	// run_test_suite(test_device, wdt_device);
	// run_test_1(test_device, wdt_device);
	run_test_2(test_device, wdt_device);
}