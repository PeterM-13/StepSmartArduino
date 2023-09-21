#include "buzzer.h"

const int buzzerPin = 4;

bool buzzerActive = false;

long buzzerTime = 0;

void setupBuzzer(){
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
}

void activateBuzzer(){
  buzzerActive = true;
}

void deactivateBuzzer(){
  buzzerActive = false;
  digitalWrite(buzzerPin, HIGH);
}

void buzzerLoop(long loopTime){
  const int buzzerDelay = alert_volume;
  if(buzzerActive){
    // if(loopTime + buzzerDelay > buzzerTime){
    //   buzzerTime = loopTime + buzzerDelay;
    //   digitalWrite(buzzerPin, !digitalRead(buzzerPin));
    // }
    digitalWrite(buzzerPin, LOW);
    showRedLeds();
    delay(buzzerDelay);
    digitalWrite(buzzerPin, HIGH);
    showLedsOff();
    delay(buzzerDelay);
  }else if(lostMode){
    digitalWrite(buzzerPin, LOW);
    showLedsOn();
    delay(150);
    digitalWrite(buzzerPin, HIGH);
    showLedsOff();
    delay(800);
  }
}