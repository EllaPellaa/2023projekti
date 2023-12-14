#include "buttons.h"
#include "arduino.h"

void initButtonsAndButtonInterrupts(void)
{

unsigned long debounceDelay = 50;

  static unsigned long last_interrupt_time1 = 0;
  static unsigned long last_interrupt_time2 = 0;
  static unsigned long last_interrupt_time3 = 0;
  static unsigned long last_interrupt_time4 = 0;
  static unsigned long last_interrupt_time5 = 0;

  int button1state;
  int button2state;
  int button3state;
  int button4state;
  int button5state;

  int lastbutton1state = LOW;
  int lastbutton2state = LOW;
  int lastbutton3state = LOW;
  int lastbutton4state = LOW;
  int lastbutton5state = LOW;

  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP); 
  pinMode(4, INPUT_PULLUP); 
  pinMode(5, INPUT_PULLUP); 
  pinMode(6, INPUT_PULLUP);

  PCICR |= B00000100;

  PCMSK2 |= B01111100;

ISR (PCINT2_vect){

  unsigned long interrupt_time1 = millis();
  unsigned long interrupt_time2 = millis();
  unsigned long interrupt_time3 = millis();
  unsigned long interrupt_time4 = millis();
  unsigned long interrupt_time5 = millis();

  int reading1 = digitalRead(2);
  int reading2 = digitalRead(3);
  int reading3 = digitalRead(4);
  int reading4 = digitalRead(5);
  int reading5 = digitalRead(6);

if (reading1 != lastbutton1state){
  last_interrupt_time1 = millis();
}
  if (reading1 != button1state){
    button1state = reading1;

    if (button1state == 0){
      if(interrupt_time1 - last_interrupt_time1 > debounceDelay){
        Serial.println("Keskeyttävä pinni on: 1");
        last_interrupt_time1 = interrupt_time1;
      }
    }
  }

if (reading2 != lastbutton2state){
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

 if (reading3 != lastbutton3state){
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

  if (reading4 != lastbutton4state){
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

  if (reading5 != lastbutton5state){
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


