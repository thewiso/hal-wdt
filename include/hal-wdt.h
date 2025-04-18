#pragma once

#include <stdbool.h>

typedef struct WDTDevice WDTDevice_t;
typedef enum WDTTimeout WDTTimeout_t;

void wdt_setup(const WDTDevice_t* device, WDTTimeout_t timeout, bool start);

bool wdt_is_running(const WDTDevice_t* device);

WDTTimeout_t wdt_get_timeout(const WDTDevice_t* device);

void wdt_start(const WDTDevice_t* device);

void wdt_stop(const WDTDevice_t* device);

void wdt_kick(const WDTDevice_t* device);

WDTDevice_t* wdt_create_device();