#pragma once

#include "../../include/hal-wdt.h"
#include "hal-test.h"

bool test_1_should_kick = true;
const TestDevice_t* test_1_test_device;
const WDTDevice_t* test_1_wdt_device;

/**
 * @brief Interne Testfunktion, nicht von außen aufrufen!
 * 
 */
void test_1_user_action()
{
	if (test_1_should_kick)
	{
		test_signal_action(test_1_test_device, watchdog_kick_stop);
		test_1_should_kick = false;
	}
}

/**
 * @brief Aktiviert WDT mit Timeout und kickt diesen regelmäßig, bis Nutzer seine Aktion ausführt. Danach sollte der Microcontroller vom WDT zum Reset gezwungen werden.
 * 
 * @param test_device 
 * @param wdt_device 
 * @param wdt_timeout 
 */
void run_test_1(const TestDevice_t* test_device, const WDTDevice_t* wdt_device, WDTTimeout_t wdt_timeout)
{
	test_1_test_device = test_device;
	test_1_wdt_device = wdt_device;

	test_setup(test_1_test_device);
	test_register_callback_user_action(test_1_test_device, test_1_user_action);
	test_signal_action(test_1_test_device, start);

	wdt_setup(test_1_wdt_device, wdt_timeout, true);
	while (test_1_should_kick)
	{
		test_delay(test_1_test_device, 200);
		wdt_kick(test_1_wdt_device);
		test_signal_action(test_1_test_device, watchdog_kick);
	}
}

bool test_2_wdt_enabled = false;
const TestDevice_t* test_2_test_device;
const WDTDevice_t* test_2_wdt_device;

/**
 * @brief Interne Testfunktion, nicht von außen aufrufen!
 * 
 */
void test_2_user_action()
{
	if (test_2_wdt_enabled)
	{
		test_signal_action(test_2_test_device, watchdog_stop);
		wdt_stop(test_2_wdt_device);
		test_2_wdt_enabled = false;
	}else{
		test_signal_action(test_2_test_device, watchdog_start);
		wdt_start(test_2_wdt_device);
		test_2_wdt_enabled = true;
	}
}

/**
 * @brief Wenn der Nutzer seine Aktion ausführt, wird der WDT aktiviert. Wenn der Nutzer erneut seine Aktion ausführt, wird der WDT wieder deaktiviert. Bei zu langer Aktivierung sollte der Microcontroller vom WDT zum Reset gezwungen werden.
 * 
 * @param test_device 
 * @param wdt_device 
 * @param wdt_timeout 
 */
void run_test_2(const TestDevice_t* test_device, const WDTDevice_t* wdt_device, WDTTimeout_t wdt_timeout){
	test_2_test_device = test_device;
	test_2_wdt_device = wdt_device;

	test_setup(test_2_test_device);
	test_register_callback_user_action(test_2_test_device, test_2_user_action);
	test_signal_action(test_2_test_device, start);

	wdt_setup(test_2_wdt_device, wdt_timeout, false);
	while (true)
	{
	}
}