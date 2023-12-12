#ifndef LEDS_H
#define LEDS_H
#include "Arduino.h"

//Kerrotaan, minkä nimisiä aliohjelmia sekä muuttujia tullaan käyttämään.

void initializeLeds();

void setLed(byte ledNumber);

void clearAllLeds(void);

void setAllLeds(void);

#endif
