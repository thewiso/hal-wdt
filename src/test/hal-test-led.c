
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "hal-test-led.h"
#include "../../include/hal-wdt.h"
#include "../../include/hal-gpio.h"

void test_setup(const TestDevice_t *device)
{
	set_pin_mode(device->green_led_pin, output);
	set_pin_mode(device->yellow_led_pin, output);
	set_pin_mode(device->red_led_pin, output);

	set_pin_mode(device->button_pin, input_pullup);
}

void test_register_callback_user_action(const TestDevice_t *device, void (*callback)())
{
	setup_interrupt(device->button_pin, falling_edge, callback);
}

void signal_start(const TestDevice_t *device)
{
	set_high(device->green_led_pin);
	_delay_ms(200);
	set_low(device->green_led_pin);
	set_high(device->yellow_led_pin);
	_delay_ms(200);
	set_low(device->yellow_led_pin);
	set_high(device->red_led_pin);
	_delay_ms(200);
	set_low(device->red_led_pin);
}

void signal_success(const TestDevice_t *device)
{
	_delay_ms(200);
	set_high(device->green_led_pin);
}

void signal_failure(const TestDevice_t *device, int test_case_number)
{
	_delay_ms(200);
	for (unsigned char i = 0; i < test_case_number; i++)
	{
		set_low(device->red_led_pin);
		_delay_ms(500);
		set_high(device->red_led_pin);
		_delay_ms(500);
	}
}

void signal_watchdog_kick(const TestDevice_t *device)
{
	toggle(device->green_led_pin);
}

void signal_watchdog_kick_stop(const TestDevice_t *device)
{
	set_high(device->red_led_pin);
}

void signal_watchdog_start(const TestDevice_t *device)
{
	set_high(device->yellow_led_pin);
}

void signal_watchdog_stop(const TestDevice_t *device)
{
	set_low(device->yellow_led_pin);
}

void test_signal_action(const TestDevice_t *device, test_action action, ...)
{
	// https://en.cppreference.com/w/c/variadic
	va_list args;
	va_start(args, action);

	switch (action)
	{
	case start:
		signal_start(device);
		break;
	case success:
		signal_success(device);
		break;
	case failure:
		signal_failure(device, va_arg(args, int)); // TODO: size of int
		break;
	case watchdog_kick:
		signal_watchdog_kick(device);
		break;
	case watchdog_kick_stop:
		signal_watchdog_kick_stop(device);
		break;
	case watchdog_start:
		signal_watchdog_start(device);
		break;
	case watchdog_stop:
		signal_watchdog_stop(device);
		break;
	}
	va_end(args);
}

void test_delay(const TestDevice_t *device, int ms)
{ // TODO: size of int
	_delay_ms((double)ms);
}

TestDevice_t *test_create_device()
{
	TestDevice_t *device = malloc(sizeof(struct TestDevice));
	device->green_led_pin = 0;
	device->yellow_led_pin = 0;
	device->red_led_pin = 0;
	device->button_pin = 0;
	return device;
}