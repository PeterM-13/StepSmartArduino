#include <FastLED.h>
#include <cmath>
#include "apiComms.h"

void setupLeds();
void showLedBatteryLevel(int timeDelay);
void showOneLed(int r, int g, int b);
void toggleLedTorch();
void showRedLeds();
void showOrangeLeds();
bool areLedsZero();
bool areLedsOff();
void switchLedsOn();
void switchLedsOff();
void showLedsOn();
void showLedsOff();
void displayHeartBeat(int bounceDuration, int startPos, int endPos);
void noWifi(int r, int b, int g);
float map(float value, float inMin, float inMax, float outMin, float outMax);