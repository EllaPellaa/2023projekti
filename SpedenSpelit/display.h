#ifndef DISPLAY_H
#define DISPLAY_H
#include <arduino.h>

void initializeDisplay(void);


void writeByte(uint8_t se_numero);




void writeHighAndLowNumber(uint8_t kympit,uint8_t ykkoset);

void showResult(byte tulos);

#endif