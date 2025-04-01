#include "../../include/hal-wdt.h"
#include "../../include/hal-wdt-ds1232.h"
#include "../../include/hal-gpio.h"
#include <stdlib.h>

bool wdt_is_timeout_valid(const WDTDevice_t* device, unsigned long timeout_ms)
{
	switch (timeout_ms)
	{
	case 150:
	case 600:
	case 1200:
		return true;
	}
	return false;
}

bool wdt_is_running(const WDTDevice_t* device) 
{
	return get_pin_mode(device->enable_rst_pin) == output && read_digital(device->enable_rst_pin) == 1;
}

// TODO check datatype, use uint32_t?
unsigned long wdt_get_timeout_ms(const WDTDevice_t* device)
{
	if(get_pin_mode(device->td_pin) == output){
		if(read_digital(device->td_pin) == 0){
			return 150;
		}else{
			return 1200;
		}
	}else{
		return 600;
	}
}

void wdt_start(const WDTDevice_t* device)
{
	//keep from resetting right after activation
	set_low(device->not_st_pin);
	set_high(device->enable_rst_pin);
	set_low(device->not_st_pin);
}

void wdt_stop(const WDTDevice_t* device)
{
	set_low(device->enable_rst_pin);
}

void wdt_kick(const WDTDevice_t* device)
{
	set_low(device->not_st_pin);
	//TODO: sleep?
	set_high(device->not_st_pin);
}

bool wdt_setup(const WDTDevice_t* device, unsigned long timeout_ms, bool start)
{
	switch (timeout_ms)
	{
	case 150:
		set_pin_mode(device->td_pin, output);
		set_low(device->td_pin);
		break;
	case 600:
		set_pin_mode(device->td_pin, input); //-> "dangling" pin
		break;
	case 1200:
		set_pin_mode(device->td_pin, output);
		set_high(device->td_pin);
		break;
	default:
		return false;
	}

	set_pin_mode(device->not_st_pin, output);
	set_high(device-> not_st_pin); //active low!

	set_pin_mode(device->enable_rst_pin, output);
	if(start){
		wdt_start(device);
	}else{
		wdt_stop(device);
	}


	return true;
}

WDTDevice_t* wdt_create_device(){
	WDTDevice_t *device = malloc(sizeof(struct WDTDevice));
	device->enable_rst_pin = 0;
	device->not_st_pin = 0;
	device->td_pin = 0;
	return device;
}