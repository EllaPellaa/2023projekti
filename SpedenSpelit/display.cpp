#include "display.h"

int shiftClock = 12; // antaa kellotaajuuden koodille
int latchClock = 14; //Kun laittaa high niin tiedot tallettuu srorageen/latch rekisteriin joka tulee sitten outputtiin
int serialInput = 17; //Syöttää dataa lol

  int numerot_bitteina [10][8]{
  {1,1,1,1,1,1,0,0}, // numero 0
  {0,1,1,0,0,0,0,0}, // numero 1
  {1,1,0,1,1,0,1,0}, // numero 2
  {1,1,1,1,0,0,1,0}, // numero 3
  {0,1,1,0,0,1,1,0}, // numero 4
  {1,0,1,1,0,1,1,0}, // numero 5
  {1,0,1,1,1,1,1,0}, // numero 6
  {1,1,1,0,0,0,0,0}, // numero 7
  {1,1,1,1,1,1,1,0}, // digit 8
  {1,1,1,1,0,1,1,0}  // numero 9
};
// Päällä, Oik.ylä, oik.ala, alla, vas.ala, vas.ylä, keskellä, 

void initializeDisplay(void)
{
pinMode(shiftClock,OUTPUT); // Aktivoidaan kaikki pinnit outputeiksi
pinMode(latchClock,OUTPUT);
pinMode(serialInput,OUTPUT);

}

void writeByte(uint8_t se_numero)
{
    digitalWrite(latchClock,LOW);  //Salpa alas

    for(int i=7; i>=0; i--){  // For loopin avulla otetaan kaikki bitit ja tutkitaan sen avulla onko kyseinen kohta bittijonossa 0 vai 1

        digitalWrite(shiftClock,LOW); // Kellotaajuus 0

        if (numerot_bitteina[se_numero][i]==1){ // Katsotaan onko annetun tietyn kohdan bitti 1 vai 0 ja sytytetään valo sen perusteella
            digitalWrite(serialInput, LOW);
        }
        if (numerot_bitteina[se_numero][i]==0){
            digitalWrite(serialInput, HIGH);
        }

        digitalWrite(shiftClock,HIGH); // Kellotaajuus ylös
  }
  digitalWrite(latchClock,HIGH); // Salpa ylös -> tallentaa muistiin sen numeron näytöllä
}


void writeHighAndLowNumber(uint8_t kympit,uint8_t ykkoset)
{
    writeByte(ykkoset);
    writeByte(kympit);
}

void showResult(byte tulos)
{ // Erotellaan kympit ja ykköset toisistaan ja lähetetään ne tulostettavaksi ruudulle
  int kympit = (tulos/10);
  int ykkoset = tulos-(kympit*10);

  writeHighAndLowNumber(kympit,ykkoset);
}