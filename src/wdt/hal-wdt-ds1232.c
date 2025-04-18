#include "../../include/hal-wdt.h"
#include "../../include/hal-wdt-ds1232.h"
#include "../gpio/hal-gpio.h"
#include <stdlib.h>

bool wdt_is_running(const WDTDevice_t* device) 
{
	return get_pin_mode(device->enable_rst_pin) == output && read_digital(device->enable_rst_pin) == 1;
}


WDTTimeout_t wdt_get_timeout(const WDTDevice_t* device)
{
	if(get_pin_mode(device->td_pin) == output){
		if(read_digital(device->td_pin) == 0){
			return ms150;
		}else{
			return ms1200;
		}
	}else{
		return ms600;
	}
}

void wdt_start(const WDTDevice_t* device)
{
	//keep from resetting right after activation
	set_low(device->not_st_pin);
	set_high(device->enable_rst_pin);
	set_high(device->not_st_pin);
}

void wdt_stop(const WDTDevice_t* device)
{
	set_low(device->enable_rst_pin);
}

void wdt_kick(const WDTDevice_t* device)
{
	set_low(device->not_st_pin);
	set_high(device->not_st_pin);
}

void wdt_setup(const WDTDevice_t* device, WDTTimeout_t timeout, bool start)
{
	switch (timeout)
	{
	case ms150:
		set_pin_mode(device->td_pin, output);
		set_low(device->td_pin);
		break;
	case ms600:
		set_pin_mode(device->td_pin, input); //-> "floating" pin
		break;
	case ms1200:
		set_pin_mode(device->td_pin, output);
		set_high(device->td_pin);
		break;
	}

	set_pin_mode(device->not_st_pin, output);
	set_high(device-> not_st_pin); //active low!

	set_pin_mode(device->enable_rst_pin, output);
	if(start){
		wdt_start(device);
	}else{
		wdt_stop(device);
	}
}

WDTDevice_t* wdt_create_device(){
	WDTDevice_t *device = malloc(sizeof(struct WDTDevice));
	device->enable_rst_pin = 0;
	device->not_st_pin = 0;
	device->td_pin = 0;
	return device;
}