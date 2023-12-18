#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include "Arduino.h"
#include "leaderboard.h"

void initLeaderboard();

void setWriting();

void setLeaderboard();

void clearLeaderBoard();

void clearCurrentLine();

void writePos();

void writeLetter();

void saveResult();

int countLetters(char arr[]);

void showLeaderboard();

void clearScreen();

void moveUp();

void moveRight();

void moveLeft();

void moveDown();

#endif