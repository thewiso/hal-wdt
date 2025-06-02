#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Gerätekonfiguration einer Test Hardware. Wird in gerätespezifischer Header-Datei definiert
 * 
 */
typedef struct TestDevice TestDevice_t;

/**
 * @brief Mögliche Aktionen, die mit test_signal_action(const TestDevice_t* device, test_action action, ...) an den Nutzer signalisiert werden können
 * 
 */
typedef enum {
	start,
	success,
	failure, //Zusätzlicher Parameter: int test_case_number
	watchdog_kick,
	watchdog_kick_stop,
	watchdog_start,
	watchdog_stop
} test_action;

/**
 * @brief Richtet Hardware mit Gerätekonfiguration ein. Muss initial aufgerufen werden, bevor irgendeine andere Funktion des HAL benutzt wird!
 * 
 * @param device Gerätekonfiguration
 */
void test_setup(const TestDevice_t* device);

/**
 * @brief Ruft callback auf, wenn Nutzer seine Aktion auslöst
 * 
 * @param device Gerätekonfiguration
 * @param callback 
 */
void test_register_callback_user_action(const TestDevice_t* device, void (*callback)());

/**
 * @brief Signalisiert die entsprechende Aktion an den Nutzer
 * 
 * @param device Gerätekonfiguration
 * @param action 
 * @param ... Spezifisch für Aktion, bitte Werte in test_action beachten!
 */
void test_signal_action(const TestDevice_t* device, test_action action, ...); //variadic function

/**
 * @brief Löst Verzögerung aus
 * 
 * @param device Gerätekonfiguration
 * @param ms 
 */
void test_delay(const TestDevice_t* device, uint32_t ms);

/**
 * @brief rstellt eine hardwarespezifische Gerätekonfiguration auf dem Heap und gibt sie zurück
 * 
 * @return TestDevice_t* 
 */
TestDevice_t* test_create_device();