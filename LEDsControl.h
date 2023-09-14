#include <FastLED.h>
#include <cmath>

void setupLeds();
void showLedBatteryLevel(int percentage, int timeDelay);
void toggleLedTorch();
void showRedLeds();
void showOrangeLeds();
bool areLedsZero();
bool areLedsOff();
void switchLedsOn();
void switchLedsOff();
void showLedsOn();
void showLedsOff();
float map(float value, float inMin, float inMax, float outMin, float outMax);