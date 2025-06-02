#pragma once

#include <stdbool.h>

/**
 * @brief Gerätekonfiguration einer WDT Hardware. Wird in gerätespezifischer Header-Datei definiert
 * 
 */
typedef struct WDTDevice WDTDevice_t;

/**
 * @brief Mögliche Timeouts einer WDT Hardware. Wird in gerätespezifischer Header-Datei definiert
 * 
 */
typedef enum WDTTimeout WDTTimeout_t;

/**
 * @brief Grundlegende Einrichtung der WDT Hardware. Muss initial aufgerufen werden, bevor irgendeine andere Funktion des HAL benutzt wird! 
 * 
 * @param device Gerätekonfiguration
 * @param timeout 
 * @param start Startet WDT direkt, wenn true
 */
void wdt_setup(const WDTDevice_t* device, WDTTimeout_t timeout, bool start);

/**
 * @brief Gibt zurück, ob WDT aktiv ist
 * 
 * @param device Gerätekonfiguration
 * @return true Wenn WDT aktiv ist
 * @return false Wenn WDT inaktiv ist
 */
bool wdt_is_running(const WDTDevice_t* device);

/**
 * @brief Gibt zurück, mit welchem Timeout der WDT konfiguriert ist.
 * 
 * @param device Gerätekonfiguration
 * @return WDTTimeout_t Undefiniert, wenn wdt_setup(const WDTDevice_t* device, WDTTimeout_t timeout, bool start) nicht vorher aufgerufen wurde
 */
WDTTimeout_t wdt_get_timeout(const WDTDevice_t* device);

/**
 * @brief Startet den WDT
 * 
 * @param device Gerätekonfiguration
 */
void wdt_start(const WDTDevice_t* device);

/**
 * @brief Stoppt den WDT
 * 
 * @param device Gerätekonfiguration
 */
void wdt_stop(const WDTDevice_t* device);

/**
 * @brief Kickt den WDT, setzt ihn zurück
 * 
 * @param device Gerätekonfiguration
 */
void wdt_kick(const WDTDevice_t* device);

/**
 * @brief Erstellt eine hardwarespezifische Gerätekonfiguration auf dem Heap und gibt sie zurück
 * 
 * @return WDTDevice_t* 
 */
WDTDevice_t* wdt_create_device();