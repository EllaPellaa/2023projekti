#include "buttons.h"
#include "arduino.h"

void initButtonsAndButtonInterrupts(void)
{

unsigned long debounceDelay = 50; //Määritellään debounceDelay ja asetetaan se 50:si. Debouncedelay kertoo nappien viiveen painalluksen jälkeen estääkseen häiriöitä.

  static unsigned long last_interrupt_time1 = 0; //Määritellään last_interrupt_time1 muuttuja, ja asetetaan se nollaksi
  static unsigned long last_interrupt_time2 = 0;
  static unsigned long last_interrupt_time3 = 0;
  static unsigned long last_interrupt_time4 = 0;
  static unsigned long last_interrupt_time5 = 0;

  int button1state; // Määritellään muuttujat
  int button2state;
  int button3state;
  int button4state;
  int button5state;

  int lastbutton1state = LOW;
  int lastbutton2state = LOW;
  int lastbutton3state = LOW;
  int lastbutton4state = LOW;
  int lastbutton5state = LOW;

  pinMode(2, INPUT_PULLUP); //Laitetaan pinnit INPUT_PULLUPiksi
  pinMode(3, INPUT_PULLUP); 
  pinMode(4, INPUT_PULLUP); 
  pinMode(5, INPUT_PULLUP); 
  pinMode(6, INPUT_PULLUP);

  PCICR |= B00000100; //Aktivoidaan keskeytykset

  PCMSK2 |= B01111100; //Valitaan mitä PCMSK2 keskeytyksistä käytetään

ISR (PCINT2_vect){

  unsigned long interrupt_time1 = millis();
  unsigned long interrupt_time2 = millis();
  unsigned long interrupt_time3 = millis();
  unsigned long interrupt_time4 = millis();
  unsigned long interrupt_time5 = millis();

  int reading1 = digitalRead(2); //Luetaan komennolla digitalRead readingX arvot
  int reading2 = digitalRead(3);
  int reading3 = digitalRead(4);
  int reading4 = digitalRead(5);
  int reading5 = digitalRead(6);

if (reading1 != lastbutton1state){  //Jos napin 1 muuttuu HIGH tilasta LOWiksi, niin seuraavat asiat tapahtuvat.
  last_interrupt_time1 = millis(); // last_interrupt_time1 muuttuu millis arvoksi
}
  if (reading1 != button1state){ //Jos reading1 ei ole sama mitä buttonstate, niin...
    button1state = reading1; // ...buttonstate muuttuu reading 1 arvoksi

    if (button1state == 0){ // Jos button1state == 0(eli LOW)...
      if(interrupt_time1 - last_interrupt_time1 > debounceDelay){ //...ja, jos interrupt_time1 - last_interrupt time1 on isompi mitä debounceDelay, niin...
        Serial.println("Keskeyttävä pinni on: 1"); //...Serial monitoriin tulostuu "Keskeytettävä pinni on:1".
        last_interrupt_time1 = interrupt_time1; // last_interrupt_time1 muuttuu interrupt_time1
      }
    }
  }

if (reading2 != lastbutton2state){ // Tässä (Napissa 2) toimii sama koodi mitä napissa 1
  last_interrupt_time2 = millis();
}
  if (reading2 != button2state){
    button2state = reading2;
  
    if (button2state == 0){
      if(interrupt_time2 - last_interrupt_time2 > debounceDelay){
        Serial.println("Keskeyttävä pinni on: 2");
        last_interrupt_time2 = interrupt_time2;
      }
    }
  }

 if (reading3 != lastbutton3state){ // Tässä (Napissa 3) toimii sama koodi mitä napissa 1
  last_interrupt_time3 = millis();
}
  if (reading3 != button3state){
    button3state = reading3;
  
    if (button3state == 0){
      if(interrupt_time3 - last_interrupt_time3 > debounceDelay){
        Serial.println("Keskeyttävä pinni on: 3");
        last_interrupt_time3 = interrupt_time3;
      }
    }
  }

  if (reading4 != lastbutton4state){ // Tässä (Napissa 4) toimii sama koodi mitä napissa 1
  last_interrupt_time4 = millis();
}
  if (reading4 != button4state){
    button4state = reading4;
  
    if (button4state == 0){
      if(interrupt_time4 - last_interrupt_time4 > debounceDelay){
        Serial.println("Keskeyttävä pinni on: 4");
        last_interrupt_time4 = interrupt_time4;
      }
    }
  }

  if (reading5 != lastbutton5state){ // Tässä (Napissa 5) toimii sama koodi mitä napissa 1
  last_interrupt_time5 = millis();
}
  if (reading5 != button5state){
    button5state = reading5;
  
    if (button5state == 0){
      if(interrupt_time5 - last_interrupt_time5 > debounceDelay){
        Serial.println("Keskeyttävä pinni on: 5");
        last_interrupt_time5 = interrupt_time5;
      }
    }
  }
}
}


