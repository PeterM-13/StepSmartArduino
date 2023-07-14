#include "LEDsControl.h"

#define LED_PIN 2
#define NUM_LEDS 54

CRGB leds[NUM_LEDS];

void setupLeds() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(255); // 0-255
  FastLED.clear();
  FastLED.show();
}

void showLedBatteryLevel(int percentage, int timeDelay){
  fill_gradient_RGB(leds, (int)map(static_cast<float>(percentage), 0.0, 100.0, 0.0, NUM_LEDS), CRGB::Red, CRGB((int)map(static_cast<float>(percentage), 0.0, 100.0, 255.0, 0.0), (int)map(static_cast<float>(percentage), 0.0, 100.0, 0.0, 255.0) ,0));
  FastLED.show();
  delay(timeDelay);
  FastLED.clear();
  FastLED.show();
}

void toggleLedTorch(){
  bool ledsOff = areLedsOff();
  if(ledsOff){
    switchLedsOn();
  }else{
    switchLedsOff();
  }
}

void switchLedsOn(){
  fill_solid(leds, NUM_LEDS, CRGB(255, 250, 240));
  FastLED.show();
}

void switchLedsOff(){
  FastLED.clear();
  FastLED.show();
}

void showRedLeds(){
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
}

bool areLedsOff() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (leds[i].r != 0 || leds[i].g != 0 || leds[i].b != 0) {
      return false;
    }
  }
  return true;
}

float map(float value, float inMin, float inMax, float outMin, float outMax) {
  // Calculate the normalized value between 0 and 1
  float normalizedValue = (value - inMin) / (inMax - inMin);
  // Map the normalized value to the output range
  float mappedValue = outMin + (normalizedValue * (outMax - outMin));
  // Return the mapped value
  return mappedValue;
}