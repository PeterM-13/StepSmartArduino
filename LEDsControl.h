#include <FastLED.h>
#include <cmath>

void setupLeds();
void showLedBatteryLevel(int percentage, int timeDelay);
void toggleLedTorch();
void showRedLeds();
bool areLedsOff();
void switchLedsOn();
void switchLedsOff();
float map(float value, float inMin, float inMax, float outMin, float outMax);