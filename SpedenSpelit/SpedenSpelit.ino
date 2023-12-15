#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

volatile int randomNumbers[100];
volatile int userNumbers[100];
volatile int currentNum = 0; 
volatile int currentUserNum = 0;
volatile bool timeToCheckGameStatus = false;

void setup()
{
  Serial.begin(9600);
  /*
    Initialize here all modules
  */
  initializeDisplay();
  initializeLeds();
  initializeTimer();

  showResult(0);
}

void loop()
{
  /*
    This loop function constantly checks if
	player has pushed a button and there is
	need for checking the game status
  */
  if(timeToCheckGameStatus == true) {
    checkGame(userNumbers[currentUserNum]);
  }
}

ISR(PCINT2_vect) {
   /*
     Here you implement logic for handling
	 interrupts from 2,3,4,5 pins for Game push buttons
	 and for pin 6 for start Game push button.
   */

   if (digitalRead(2) == LOW) {
      userNumbers[currentUserNum] = 0;
      timeToCheckGameStatus = true;
   } else if (digitalRead(3) == LOW) {
      userNumbers[currentUserNum] = 1;
      timeToCheckGameStatus = true;
   } else if (digitalRead(4) == LOW) {
      userNumbers[currentUserNum] = 2;
      timeToCheckGameStatus = true;
   } else if (digitalRead(5) == LOW) {
      userNumbers[currentUserNum] = 3;
      timeToCheckGameStatus = true;
   }

   if (digitalRead(6) == LOW) {
    startTheGame();
   }
}


ISR(TIMER1_COMPA_vect)
{
  
  if ((currentNum % 10) == 0) {
    OCR1A -= OCR1A * 0.1;
  }

  randomNumbers[currentNum] = random(0,4);
  clearAllLeds();
  setLed(randomNumbers[currentNum]);
  currentNum++;
  /*
    Here you generate a random number that is used to
	set certain led.
	
	Each timer interrupt increments "played" numbers,
	and after each 10 numbers you must increment interrupt
	frequency.
	
	Each generated random number must be stored for later
	comparision against player push button presses.
  */
  
}



void initializeTimer(void)
{
	// see requirements for the function from SpedenSpelit.h

  //Nappien keskeytykset, korjataan binäärihomma kun tiedossa kaikki käytettävät pinnit
  PCICR |= B00000100; //Sallitaan keskeytykset
  PCMSK2 |= B11111100; //Portit 2-7 aktiivisiksi, muutetaan tarvittaessa

  //Timer
  TCCR1A = 0; //Resetataan tämä nollaksi
  TCCR1B = 0; //Tämä myös

  TCCR1B |= B00000100; //Prescaler 256

  //Lasketaan tähän luku, johon asti timerin pitää laskea sekunnin keskeytyksen saamiseksi
  OCR1A = 62500;
  TCNT1  = 0;
}

void checkGame(byte nbrOfButtonPush)
{
	// see requirements for the function from SpedenSpelit.h
  if (randomNumbers[currentUserNum] != nbrOfButtonPush) {
    stopTheGame();
    timeToCheckGameStatus = false;
  } else if (randomNumbers[currentUserNum] == nbrOfButtonPush){
    currentUserNum++;
    showResult(currentUserNum);
    timeToCheckGameStatus = false;
  }
}


void initializeGame()
{

	// see requirements for the function from SpedenSpelit.h
  randomNumbers[100] = 0;
  userNumbers[100] = 0;
  currentNum = 0; 
  currentUserNum = 0;
  showResult(0);
}

void startTheGame()
{
  TIMSK1 |= B00000010; //Enabloidaan compare match/keskeytykset
  initializeGame();
   // see requirements for the function from SpedenSpelit.h
}

void stopTheGame() {
  TIMSK1 |= B00000000; //Disabloidaan compare match/keskeytykset
  TCNT1  = 0;
  OCR1A = 62500;
}

