// #include "../include/hal-wdt-internal-atmega-328p.h"
#include "../include/hal-wdt-ds1232.h"
#include "../include/hal-gpio-atmega-328p.h"
#include "test/hal-test-led.h"
#include "test/unit-test.h"
#include "test/integration-test.h"

int main()
{
	TestDevice_t* test_device = test_create_device();

	test_device->green_led_pin = PD0;
	test_device->yellow_led_pin = PD1;
	test_device->red_led_pin = PD2;
	test_device->button_pin = PD3;

	WDTDevice_t* wdt_device = wdt_create_device();
	wdt_device->enable_rst_pin = PC3;
	wdt_device->td_pin = PC4;
	wdt_device->not_st_pin = PC5;

	WDTTimeout_t timeout = ms1200;

	// run_test_suite(test_device, wdt_device, timeout);
	// run_test_1(test_device, wdt_device, timeout);
	run_test_2(test_device, wdt_device, timeout);
}