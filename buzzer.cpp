#include "buzzer.h"

const int buzzerPin = 4;

bool buzzerActive = false;

const int buzzerDelay = alert_volume;
long buzzerTime = 0;

void setupBuzzer(){
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
}

void activateBuzzer(){
  buzzerActive = true;
}

void deactivaeBuzzer(){
  buzzerActive = false;
  digitalWrite(buzzerPin, 0);
}

void buzzerLoop(long loopTime){
  if(buzzerActive){
    if(loopTime + buzzerDelay > buzzerTime){
      buzzerTime = loopTime + buzzerDelay;
      digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    }
  }
}