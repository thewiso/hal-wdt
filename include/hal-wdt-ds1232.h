#pragma once

struct WDTDevice //TODO: in source datei
{
	int enable_rst_pin; //used to provide DS1232 with power
	int td_pin; //used for timeout period //TODO: int type! 
	int not_st_pin; //used for kicking wdt
};
