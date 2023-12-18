#include "leaderboard.h"
#include "Arduino.h"
#include "TFT.h"
#include "SPI.h"

int countLetters(char arr[]) {
    int maara = 0;
    for (int i = 0; i < 6; i++) {
        if (arr[i] != 0) {
        maara++;
        }
    }
    return maara;
}