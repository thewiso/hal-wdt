#pragma once
#include "../../include/hal-wdt.h"
#include "hal-test.h"

#define TEST_SUCCESS true
#define TEST_FAILURE false
#define TEST_RESULT bool

#define ASSERT(CONDITION) \
	if (!(CONDITION))     \
		return TEST_FAILURE;


WDTTimeout_t timeout;

TEST_RESULT test_setup_and_start(const WDTDevice_t* device)
{
	wdt_setup(device, timeout, true);
	ASSERT(wdt_is_running(device));
	ASSERT(wdt_get_timeout(device) == timeout);
	wdt_stop(device);
	ASSERT(!wdt_is_running(device));
	ASSERT(wdt_get_timeout(device) == timeout);

	return TEST_SUCCESS;
}

TEST_RESULT test_setup_valid_and_start_later(const WDTDevice_t* device)
{
	wdt_setup(device, timeout, false);
	ASSERT(!wdt_is_running(device));
	ASSERT(wdt_get_timeout(device) == timeout);
	wdt_start(device);
	ASSERT(wdt_is_running(device));
	ASSERT(wdt_get_timeout(device) == timeout);
	wdt_stop(device);
	ASSERT(!wdt_is_running(device));
	ASSERT(wdt_get_timeout(device) == timeout);
	return TEST_SUCCESS;
}

#define TEST_COUNT 2
static TEST_RESULT (*test_suite[TEST_COUNT])(const WDTDevice_t* device) = {
										  test_setup_and_start,
										  test_setup_valid_and_start_later};

void run_test_suite(const TestDevice_t* test_device, const WDTDevice_t* wdt_device, WDTTimeout_t wdt_timeout)
{
	timeout = wdt_timeout;
	test_setup(test_device);
	test_signal_action(test_device, start);

	bool test_success = true;
	unsigned short i = 0;
	for (; i < TEST_COUNT; i++)
	{
		TEST_RESULT result = test_suite[i](wdt_device);
		if (result == TEST_FAILURE)
		{
			test_success = false;
			break;
		}
	}

	if (test_success)
	{
		test_signal_action(test_device, success);
	}
	else
	{
		test_signal_action(test_device, failure, i + 1);
	}
}
