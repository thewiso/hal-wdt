#pragma once

#include <stdbool.h>

typedef struct TestDevice TestDevice_t;

typedef enum {
	start,
	success,
	failure,
	watchdog_kick,
	watchdog_kick_stop,
	watchdog_start,
	watchdog_stop
} test_action;

void test_setup(const TestDevice_t* device);

void test_register_callback_user_action(const TestDevice_t* device, void (*callback)());

void test_signal_action(const TestDevice_t* device, test_action action, ...); //variadic function

void test_delay(const TestDevice_t* device, int ms);

TestDevice_t* test_create_device();