#include <FastLED.h>
#include <cmath>

float map(float value, float inMin, float inMax, float outMin, float outMax);
void showLedBatteryLevel(int percentage);
void toggleLedTorch();
void setupLeds();
bool areLedsOff();