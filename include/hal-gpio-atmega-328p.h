#pragma once

#define PORT_B 0
#define PB0 PIN_ID(PORT_B, 0) 
#define PB1 PIN_ID(PORT_B, 1)
#define PB2 PIN_ID(PORT_B, 2)
#define PB3 PIN_ID(PORT_B, 3)
#define PB4 PIN_ID(PORT_B, 4)
#define PB5 PIN_ID(PORT_B, 5)
#define PB6 PIN_ID(PORT_B, 6)
#define PB7 PIN_ID(PORT_B, 7)

#define PORT_C 1
#define PC0 PIN_ID(PORT_C, 0)
#define PC1 PIN_ID(PORT_C, 1)
#define PC2 PIN_ID(PORT_C, 2)
#define PC3 PIN_ID(PORT_C, 3)
#define PC4 PIN_ID(PORT_C, 4)
#define PC5 PIN_ID(PORT_C, 5)
#define PC6 PIN_ID(PORT_C, 6)

#define PORT_D 2
#define PD0 PIN_ID(PORT_D, 0)
#define PD1 PIN_ID(PORT_D, 1)
#define PD2 PIN_ID(PORT_D, 2)
#define PD3 PIN_ID(PORT_D, 3)
#define PD4 PIN_ID(PORT_D, 4)
#define PD5 PIN_ID(PORT_D, 5)
#define PD6 PIN_ID(PORT_D, 6)
#define PD7 PIN_ID(PORT_D, 7)

#define PIN_ID(port, pin)  (((port) << 8) | (pin))
