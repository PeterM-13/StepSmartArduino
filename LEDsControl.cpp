#include "LEDsControl.h"

#define LED_PIN 6
#define NUM_LEDS 54

const int ledControlPin = 5;

CRGB leds[NUM_LEDS];

void setupLeds() {
  pinMode(ledControlPin, OUTPUT);
  switchLedsOn();
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(255); // 0-255
  FastLED.clear();
  FastLED.show();
}

void showLedBatteryLevel(int timeDelay){
  fill_gradient_RGB(leds, (int)map(static_cast<float>(battery), 0.0, 100.0, 0.0, NUM_LEDS), CRGB::Red, CRGB((int)map(static_cast<float>(battery), 0.0, 100.0, 255.0, 0.0), (int)map(static_cast<float>(battery), 0.0, 100.0, 0.0, 255.0) ,0));
  FastLED.show();
  delay(timeDelay);
  FastLED.clear();
  FastLED.show();
}

void showOneLed(int r, int g, int b){
  CRGB colour = CRGB(r, g, b);
  leds[0] = colour;
  //leds[NUM_LEDS-1] = colour;
  FastLED.show();
}

void toggleLedTorch(){
  bool ledsOff = areLedsOff();
    if(ledsOff){
    switchLedsOn();
  }else{
    switchLedsOff();
  }
  bool ledsZero = areLedsZero();
  if(ledsZero){
    showLedsOn();
  }else{
    showLedsOff();
  }
}

void switchLedsOn(){
  digitalWrite(ledControlPin, HIGH);
}

void switchLedsOff(){
  digitalWrite(ledControlPin, LOW);
}

void showLedsOn(){
  fill_solid(leds, NUM_LEDS, CRGB(255, 250, 240));
  FastLED.show();
}

void showLedsOff(){
    FastLED.clear();
    FastLED.show();
}

void showRedLeds(){
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
}

void showOrangeLeds(){
  fill_solid(leds, NUM_LEDS, CRGB(255, 140, 0));
  FastLED.show();
}

bool areLedsZero() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (leds[i].r != 0 || leds[i].g != 0 || leds[i].b != 0) {
      return false;
    }
  }
  return true;
}

bool areLedsOff(){
  return digitalRead(ledControlPin);
}

void displayHeartBeat(int bounceDuration=3, int startPos=0, int endPos=NUM_LEDS - 1) {
  CRGB orange = CRGB(255, 165, 0);
  int numBounces = 1;
  // int bounceDuration = 5;
  // int startPos = 0;
  // int endPos = NUM_LEDS - 1;

  for (int bounce = 0; bounce < numBounces; bounce++) {
    for (int i = startPos; i <= endPos; i++) {
      leds[i] = orange;
      FastLED.show();
      delay(bounceDuration);
    }
    
    for (int i = endPos; i >= startPos; i--) {
      leds[i] = CRGB::Black;
      FastLED.show();
      delay(bounceDuration);
    }

    startPos = (startPos + 1) % NUM_LEDS;
    endPos = (endPos - 1 + NUM_LEDS) % NUM_LEDS;
  }
  
  FastLED.clear();
  FastLED.show();
}


void noWifi(int r, int b, int g) {
  CRGB orange = CRGB(r, b, g);
  int numIterations = NUM_LEDS / 2;
  int spreadDelay = 10;
  for(int j = 0; j<1; j++){
    for (int i = 0; i < numIterations; i++) {
      leds[NUM_LEDS / 2 + i] = orange;
      leds[NUM_LEDS / 2 - i - 1] = orange;
      FastLED.show();
      delay(spreadDelay);
    }

    for (int i = 0; i < numIterations; i++) {
      leds[NUM_LEDS / 2 + i] = CRGB::Black;
      leds[NUM_LEDS / 2 - i - 1] = CRGB::Black;
      FastLED.show();
      delay(spreadDelay);
    }
  }

  FastLED.clear();
  FastLED.show();
}


float map(float value, float inMin, float inMax, float outMin, float outMax) {
  // Calculate the normalized value between 0 and 1
  float normalizedValue = (value - inMin) / (inMax - inMin);
  // Map the normalized value to the output range
  float mappedValue = outMin + (normalizedValue * (outMax - outMin));
  // Return the mapped value
  return mappedValue;
}