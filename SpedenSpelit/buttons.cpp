#include "buttons.h"
#include "arduino.h"

void initButtonsAndButtonInterrupts(void)
{
  pinMode(A10, INPUT_PULLUP);  //Laitetaan pinnit INPUT_PULLUPiksi
  pinMode(A11, INPUT_PULLUP); 
  pinMode(A12, INPUT_PULLUP); 
  pinMode(A13, INPUT_PULLUP); 
  pinMode(A14, INPUT_PULLUP);
}
