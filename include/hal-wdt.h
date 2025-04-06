#pragma once

#include <stdbool.h>

typedef struct WDTDevice WDTDevice_t;

bool wdt_setup(const WDTDevice_t* device, unsigned long timeout_ms, bool start);

bool wdt_is_timeout_valid(const WDTDevice_t* device, unsigned long timeout_ms);

bool wdt_is_running(const WDTDevice_t* device);

unsigned long wdt_get_timeout_ms(const WDTDevice_t* device);

void wdt_start(const WDTDevice_t* device);

void wdt_stop(const WDTDevice_t* device);

void wdt_kick(const WDTDevice_t* device);

WDTDevice_t* wdt_create_device(); //TODO: in header datei von internahl oder ds1232 mit parametern!