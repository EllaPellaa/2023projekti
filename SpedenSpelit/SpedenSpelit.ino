#include <TFT.h>  
#include <SPI.h>
#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"
#include "leaderboard.h"
#include <EEPROM.h>

volatile int randomNumbers[100];
volatile int userNumbers[100];
volatile int currentNum = 0; 
volatile int currentUserNum = 0;
volatile bool timeToCheckGameStatus = false;
volatile bool timeToCheckLeds = false;
volatile int gameStatus = 0;

//Nappikeskeytyskoodi
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

//leaderboard
volatile int tila[2] = {0, 0};
char name[10];
char leaderBoard[5][2][7];
char kirjaimet[4][7] = {{'A', 'B', 'C', 'D', 'E', 'F', 'G'}, {'H', 'I', 'J', 'K', 'L', 'M', 'N'}, {'O', 'P', 'Q', 'R', 'S', 'T', 'U'}, {'V', 'W', 'X', 'Y', 'Z'}};

// Nämä muutetaan, pinnit arduinoon
#define cs   10
#define dc   9
#define rst  8
TFT TFTscreen = TFT(cs, dc, rst);

void setup()
{
  Serial.begin(9600);
  /*
    Initialize here all modules
  */
  initializeDisplay();
  initializeLeds();
  initializeTimer();
  initButtonsAndButtonInterrupts();
  initLeaderboard();
  showLeaderboard();
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

  if(timeToCheckLeds == true && TCNT1 == 6200) {
    setLed(randomNumbers[currentNum]);
    currentNum++;
    timeToCheckLeds = false;
  }
}

ISR(PCINT2_vect) {
   /*
     Here you implement logic for handling
	 interrupts from 2,3,4,5 pins for Game push buttons
	 and for pin 6 for start Game push button.
   */
    unsigned long interrupt_time1 = millis();
    unsigned long interrupt_time2 = millis(); 
    unsigned long interrupt_time3 = millis();
    unsigned long interrupt_time4 = millis();
    unsigned long interrupt_time5 = millis();

    int reading1 = digitalRead(A10); //Luetaan komennolla digitalRead readingX arvot
    int reading2 = digitalRead(A11);
    int reading3 = digitalRead(A12);
    int reading4 = digitalRead(A13);
    int reading5 = digitalRead(A14);

    if (reading1 != lastbutton1state){  //Jos napin 1 muuttuu HIGH tilasta LOWiksi, niin seuraavat asiat tapahtuvat.
      last_interrupt_time1 = millis();  // last_interrupt_time1 muuttuu millis arvoksi
    }
      if (reading1 != button1state){ //Jos reading1 ei ole sama mitä buttonstate, niin...
        button1state = reading1; // ...buttonstate muuttuu reading 1 arvoksi

        if (button1state == 0){  // Jos button1state == 0(eli LOW)...
          if(interrupt_time1 - last_interrupt_time1 > debounceDelay){  //...ja, jos interrupt_time1 - last_interrupt time1 on isompi mitä debounceDelay, niin...
            Serial.println("Keskeyttävä pinni on: 2");  //...Serial monitoriin tulostuu "Keskeytettävä pinni on:2".
            last_interrupt_time1 = interrupt_time1; // last_interrupt_time1 muuttuu interrupt_time1
            if (gameStatus == 1 && currentUserNum < currentNum) {
              userNumbers[currentUserNum] = 0;
              timeToCheckGameStatus = true;
            } else if(gameStatus == 2) {
              moveLeft();
            }
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
            Serial.println("Keskeyttävä pinni on: 3");
            if (gameStatus == 1 && currentUserNum < currentNum) {
              userNumbers[currentUserNum] = 1;
              timeToCheckGameStatus = true;
            } else if(gameStatus == 2) {
              moveUp();
            }
            last_interrupt_time2 = interrupt_time2;
          }
        }
      }

    if (reading3 != lastbutton3state){  // Tässä (Napissa 3) toimii sama koodi mitä napissa 1
      last_interrupt_time3 = millis();
    }
      if (reading3 != button3state){
        button3state = reading3;
      
        if (button3state == 0){
          if(interrupt_time3 - last_interrupt_time3 > debounceDelay){  
            Serial.println("Keskeyttävä pinni on: 4");
            if(gameStatus == 1 && currentUserNum < currentNum) {
              userNumbers[currentUserNum] = 2;
              timeToCheckGameStatus = true;
            } else if(gameStatus == 2) {
              moveDown();
            }
            last_interrupt_time3 = interrupt_time3;
          }
        }
      }

      if (reading4 != lastbutton4state){  // Tässä (Napissa 4) toimii sama koodi mitä napissa 1
      last_interrupt_time4 = millis();
    }
      if (reading4 != button4state){
        button4state = reading4;
      
        if (button4state == 0){
          if(interrupt_time4 - last_interrupt_time4 > debounceDelay){
            Serial.println("Keskeyttävä pinni on: 5");
            if(gameStatus == 1 && currentUserNum < currentNum) {
              userNumbers[currentUserNum] = 3;
              timeToCheckGameStatus = true;
            } else if(gameStatus == 2) {
              moveRight();
            }
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
            Serial.println("Keskeyttävä pinni on: 6");
            if(gameStatus == 0) {
              startTheGame();
            } else if(gameStatus == 2) {
              writeLetter();
            } else if(gameStatus == 3) {
              startTheGame();
            }
            last_interrupt_time5 = interrupt_time5;
          }
        }
    }
}


ISR(TIMER1_COMPA_vect)
{
  TCNT1  = 0; 
  if(currentUserNum == 99) {
    stopTheGame();
    setWriting();
    timeToCheckGameStatus = false;
  }

  if ((currentNum % 10) == 0 && currentNum != 0) {
    OCR1A *= 0.88;
  }
  clearAllLeds();
  //Tästä siirretty ledihomma looppiin, jotta pieni tauko valon palaessa
  randomNumbers[currentNum] = random(0,4);
  timeToCheckLeds = true;
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

  TIMSK1 &= B00000000; //Nämä pois päältä

  //Lasketaan tähän luku, johon asti timerin pitää laskea sekunnin keskeytyksen saamiseksi
  OCR1A = 62500;
  TCNT1  = 0;
}

void checkGame(byte nbrOfButtonPush)
{
	// see requirements for the function from SpedenSpelit.h
  if (randomNumbers[currentUserNum] != nbrOfButtonPush) {
    stopTheGame();
    setWriting();
    timeToCheckGameStatus = false;
  } else if(currentUserNum == 99) {
    stopTheGame();
    setWriting();
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
  tila[0] = 0;
  tila[1] = 0;
  for (int i = 0; i < 10; i++) {
    name[i] = 0;
  }
  gameStatus = 1;
  initializeGame();
  TIMSK1 |= B00000010; //Enabloidaan compare match/keskeytykset
   // see requirements for the function from SpedenSpelit.h
}

void stopTheGame() {
  gameStatus = 2;
  clearAllLeds();
  TIMSK1 &= B00000000; //Disabloidaan compare match/keskeytykset
  TCNT1 = 0;
  OCR1A = 62500;
}

//Leaderboardin funktiota

void moveUp() {
  clearCurrentLine();
  if (tila[0] > 0)
  tila[0] -= 1;
  writePos();
}

void moveLeft() {
  clearCurrentLine();
  if (tila[1] > 0)
  tila[1] -= 1;
  writePos();
}

void moveDown() {
  clearCurrentLine();

  if (tila[0] < 3) {
    tila[0] += 1;
  }

  if(tila[0] == 3 && tila[1] > 5) {
    tila[1] = 5;
  }
  writePos();
}

void moveRight() {
  clearCurrentLine();
  if (tila[1] < 6 && tila[0] != 3) {
    tila[1] += 1;
    writePos();
  } else if (tila[0] == 3 && tila[1] < 5) {
    tila[1] += 1;
    writePos();
  } else {
    writePos();
  }
}

void writePos() {
    int y = tila[0];
    int x = tila[1];
    int x2 = 0;

    if (x == 0) {
    x = 1;
    x2 = x + 14;
    } else if (x == 1) {
    x = 25;
    x2 = x + 14;
    } else if (x == 2) {
    x = 49;
    x2 = x + 14;
    } else if (x == 3) {
    x = 73;
    x2 = x + 14;
    } else if (x == 4) {
    x = 97;
    x2 = x + 14;
    } else if (x == 5) {
    x = 121;
    x2 = x + 14;
    } else if (x == 6) {
    x = 145;
    x2 = x + 14;
    }

    if (y == 0) {
    y = 41;
    } else if (y == 1) {
    y = 65;
    } else if (y == 2) {
    y = 89;
    } else if (y == 3) {
    y = 113;
    }

    TFTscreen.line(x,y,x2,y);
}

void writeLetter() {
    if(gameStatus == 2) {
        if (tila[0] == 3 && tila[1] == 5) {
        saveResult();
        } else {
          if(countLetters(name) != 6) {
              char letter = kirjaimet[tila[0]][tila[1]];
              int numOfLetter = countLetters(name);
              name[numOfLetter] = letter;
              TFTscreen.text(name, 3, 3);
          }
        }
    }
}

//Tallennetaan tulokset
void saveResult() {
    char numArr[16];
    //Jos paras tulos, jokainen if yhtä rankkia alemmalle
    if(atoi(leaderBoard[0][1]) < currentUserNum) {
        strcpy(leaderBoard[4][0], leaderBoard[3][0]);
        strcpy(leaderBoard[4][1], leaderBoard[3][1]);
        strcpy(leaderBoard[3][0], leaderBoard[2][0]);
        strcpy(leaderBoard[3][1], leaderBoard[2][1]);
        strcpy(leaderBoard[2][0], leaderBoard[1][0]);
        strcpy(leaderBoard[2][1], leaderBoard[1][1]);
        strcpy(leaderBoard[1][0], leaderBoard[0][0]);
        strcpy(leaderBoard[1][1], leaderBoard[0][1]);
        strcpy(leaderBoard[0][1], itoa(currentUserNum, numArr, 10));
        strcpy(leaderBoard[0][0], name);
        Serial.println("TEST");
    } else if(atoi(leaderBoard[1][1]) < currentUserNum) {
        strcpy(leaderBoard[4][0], leaderBoard[3][0]);
        strcpy(leaderBoard[4][1], leaderBoard[3][1]);
        strcpy(leaderBoard[3][0], leaderBoard[2][0]);
        strcpy(leaderBoard[3][1], leaderBoard[2][1]);
        strcpy(leaderBoard[2][0], leaderBoard[1][0]);
        strcpy(leaderBoard[2][1], leaderBoard[1][1]);
        strcpy(leaderBoard[1][1], itoa(currentUserNum, numArr, 10));
        strcpy(leaderBoard[1][0], name);
    } else if(atoi(leaderBoard[2][1]) < currentUserNum) {
        strcpy(leaderBoard[4][0], leaderBoard[3][0]);
        strcpy(leaderBoard[4][1], leaderBoard[3][1]);
        strcpy(leaderBoard[3][0], leaderBoard[2][0]);
        strcpy(leaderBoard[3][1], leaderBoard[2][1]);
        strcpy(leaderBoard[2][1], itoa(currentUserNum, numArr, 10));
        strcpy(leaderBoard[2][0], name);
    } else if(atoi(leaderBoard[3][1]) < currentUserNum) {
        strcpy(leaderBoard[4][0], leaderBoard[3][0]);
        strcpy(leaderBoard[4][1], leaderBoard[3][1]);
        strcpy(leaderBoard[3][1], itoa(currentUserNum, numArr, 10));
        strcpy(leaderBoard[3][0], name);
    } else if(atoi(leaderBoard[4][1]) < currentUserNum) {
        strcpy(leaderBoard[4][1], itoa(currentUserNum, numArr, 10));
        strcpy(leaderBoard[4][0], name);
    }
    writeLeaderboardToEEPROM();
    gameStatus = 3;
    showLeaderboard();
}

void setWriting() {
    noInterrupts();
    TFTscreen.background(0, 0, 0);
    //write letters and line
    TFTscreen.line(0,20,160,20);
    TFTscreen.text("A B C D E F G", 3, 26);
    TFTscreen.text("H I J K L M N", 3, 50);
    TFTscreen.text("O P Q R S T U", 3, 74);
    TFTscreen.text("V W X Y Z >", 3, 98);

    //start line
    TFTscreen.line(1,41,15,41);
    interrupts();
}

void clearCurrentLine() {

    TFTscreen.stroke(0, 0, 0);

    int y = tila[0];
    int x = tila[1];
    int x2 = 0;

    if (x == 0) {
    x = 1;
    x2 = x + 14;
    } else if (x == 1) {
    x = 25;
    x2 = x + 14;
    } else if (x == 2) {
    x = 49;
    x2 = x + 14;
    } else if (x == 3) {
    x = 73;
    x2 = x + 14;
    } else if (x == 4) {
    x = 97;
    x2 = x + 14;
    } else if (x == 5) {
    x = 121;
    x2 = x + 14;
    } else if (x == 6) {
    x = 145;
    x2 = x + 14;
    }

    if (y == 0) {
    y = 41;
    } else if (y == 1) {
    y = 65;
    } else if (y == 2) {
    y = 89;
    } else if (y == 3) {
    y = 113;
    }
    
    TFTscreen.line(x,y,x2,y);
    
    TFTscreen.stroke(255, 255, 255);
}

void showLeaderboard() {
  TFTscreen.background(0, 0, 0);
  TFTscreen.text("NAME", 3, 3);
  TFTscreen.text("SCORE", 100, 3);
  TFTscreen.line(0,20,160,20);
  TFTscreen.text(leaderBoard[0][0], 3, 26);
  TFTscreen.text(leaderBoard[0][1], 130, 26);
  TFTscreen.text(leaderBoard[1][0], 3, 46);
  TFTscreen.text(leaderBoard[1][1], 130, 46);
  TFTscreen.text(leaderBoard[2][0], 3, 66);
  TFTscreen.text(leaderBoard[2][1], 130, 66);
  TFTscreen.text(leaderBoard[3][0], 3, 86);
  TFTscreen.text(leaderBoard[3][1], 130, 86);
  TFTscreen.text(leaderBoard[4][0], 3, 106);
  TFTscreen.text(leaderBoard[4][1], 130, 106);
}

void clearScreen() {
  TFTscreen.background(0, 0, 0);
}

void initLeaderboard() {
    TFTscreen.begin();

    readLeaderboardFromEEPROM();

    // clear the screen with a black background
    TFTscreen.background(0, 0, 0);
    //set the text size
    TFTscreen.setTextSize(2);
    //set text color
    TFTscreen.stroke(255, 255, 255);
}

// ***************************
// EEPROM TALLENNUSJUTUT
// ***************************

// TALLENNUS EEPROMIIN
// *******************

void writeLeaderboardToEEPROM(){

  int nimiAddress = 1;            // Index sija 2, josta lähtien EEPROMin sisälle kirjoitetaan indexiin 7 saakka.
  int pisteAddress = 8;         // Index sija 8, johon pisteet tallennetaan int muodossa.
  byte length = 7;          // nimen merkkipituus
  char nimi[7];
  for(int x = 0; x<5; x++){   // suoritetaan viisi kertaa; kirjoitetaan EEPROMiin kaikki taulukon viisi tulosta.

    strcpy(nimi,leaderBoard[x][0]);  //napataan nimi taulukosta  

    int pisteet;
    pisteet = atoi(leaderBoard[x][1]);  //.. ja pistetulos

    for(int i = 0; i<length; i++){  // Kirjoitetaan nimi EEPROMiin tavu kerrallaan, eli aina 7 kertaa
    
      EEPROM.update(nimiAddress + i, nimi[i]);
    }
    EEPROM.update(pisteAddress,pisteet);  //.. jonka jälkeen kirjoitetaan pisteet.

    nimiAddress+=8;     // Lisätään 
    pisteAddress+=8;
  
  }

  }
//0 "1 2 3 4 5 6 7" 8
// tavut, joita käytetään eepromissa (yht. 7 tekstiä, 1 on numerot)

// TIETOJEN SIIRTO TAULUKKOON
// ***************************

void readLeaderboardFromEEPROM(){
  int nimiAddress = 1;    //tallentavien ensimmäinen tavu
  int pisteAddress = 8;  //kahdeksas tavu
  byte length = 7;
  char numArray[16];
  for(int index=0; index<5; index++){
    char nimi[7];
    
    for(int x=0; x<length; x++){
      nimi[x] = EEPROM.read(nimiAddress + x);
    }
    
    int pisteet = EEPROM.read(pisteAddress);
    
    strcpy(leaderBoard[index][0], nimi);
    strcpy(leaderBoard[index][1], itoa(pisteet, numArray, 10));
    nimiAddress+=8;
    pisteAddress+=8;
  }


}

    /* SELECT * FROM Leaderboard_ledinaytto WHERE ManneApproved = TRUE

              ⨻This query returned no results.
    

                              _____________
                             /             \
                                  --    ---
                            | --   u      u  
                             )   \___/  \___/
                                      >                   
                                  \_______/              
                                                        
    */


