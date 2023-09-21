#include <arduino-timer.h>

#include "apiComms.h"
#include "LEDsControl.h"
#include "buzzer.h"
#include "fallDetection.h"
#include "HeartMonitoring.h"

const int fetchDelay = 20;       // every 20s, update with data from cloud (remove 0 from each)

auto fetchTimer = timer_create_default();
auto alert_fallTimer = timer_create_default();
auto alert_alarmTimer = timer_create_default();

// Interupts: 2, 3, 9, 10, 11, 13, A1, A5, A7
// Digiatl pins
const int ledSwitchPin = 2;
const int emergencyButtonPin = 3;
//const int ledControlPin = 5
// Analouge pins
const int batteryPin = A1;//15;

int alertStage = 0; // 0=off, 1=wating_for_lift-up, 2=buzzer&lights, 3=notify
bool fallDetected = false;

bool ledSwitchState = LOW;
bool emergencyButtonState = LOW;

void setup() {
  // Serial.begin(9600);
  // while (!Serial) {
  //  ; // Wait for serial port to connect
  // }
  //pinMode(LED_BUILTIN, OUTPUT);

  // Setup Pins
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledSwitchPin, INPUT_PULLUP);
  pinMode(emergencyButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(emergencyButtonPin), emergencySwitch, FALLING);
  attachInterrupt(digitalPinToInterrupt(ledSwitchPin), ledSwitch, CHANGE);

  // Setup functions
  setupLeds();
  setupBuzzer();
  setupFallDetection();

  connectToWiFi();

  // Read battery level on start up
  readBatteryLevel();
  showLedBatteryLevel(3000);

  // Setup timers
  fetchTimer.every(fetchDelay*1000, fetchTimerCallback);
}


void loop() {
  const long loopTime = millis();  // Time since board started program in ms
  
  if(!emergency){
    if(alertStage == 0 && detectFall()){
      emergency = true;
      fallDetected = true;
      showRedLeds();
      alert_fallTimer.in(alert_fall*1000, alert_fallTimerCallback);
      alertStage = 1;
    }
    if(online){
      fetchTimer.tick();
    }
    if(heartLogging){
      heartLoop((!ledSwitchState && !emergency));
    }
  }else if(emergency) {
    alert_fallTimer.tick();
    alert_alarmTimer.tick();
    if(alertStage == 1 && detectLift()){
      emergency = false;
      fallDetected = false;
      alertStage = 0;
      showLedsOff();
      alert_fallTimer.cancel();
    }
  }

  buzzerLoop(loopTime);
}

// --- Timer functions --- //
bool fetchTimerCallback(void *){
  //Serial.println("Make API request");
  makeAPIRequest();
  if(ledSwitchState){
    showOneLed(255, 255, 255);
  }else{
    showOneLed(0, 0, 0);
  }
  return true;
}
bool alert_fallTimerCallback(void *){
  activateBuzzer();
  showRedLeds();
  alertStage = 2;
  alert_alarmTimer.in(alert_alarm*1000, alert_alarmTimerCallback);
  return false;
}
bool alert_alarmTimerCallback(void *){
  //Serial.println("Notifying");
  if(alerting && online){
    sendEmergencyDataToAPI(fallDetected);
  }
  showLedsOff();
  deactivateBuzzer();
  emergency = false;
  return false;
}

// --- Other Functions --- //
void emergencySwitch(){
  emergency = !emergency;
  if(emergency){
    alertStage = 2;
    activateBuzzer();
    showRedLeds();
    alert_alarmTimer.in(alert_alarm*1000, alert_alarmTimerCallback);
  }else{
    showLedsOff();
    deactivateBuzzer();
    alert_alarmTimer.cancel();
    alert_fallTimer.cancel();
    alertStage = 0;
    emergency = false;
    fallDetected = false;
  }
  delay(20);
}

void ledSwitch(){
  ledSwitchState = !ledSwitchState;
  toggleLedTorch();
  delay(20);
}

long totalBattery = 0;
int outputBattery = 0;
int numSamples = 600;
//int ref = 0;
void readBatteryLevel(){
  //int perc = battery;
  for(int i=0; i<numSamples;i++){
    int val = analogRead(batteryPin);  // read the input pin
    totalBattery += val;
    outputBattery = int(totalBattery/100);
    totalBattery -= outputBattery;
  }
  float volts = (outputBattery * 0.003222) / 0.75;
  int perc = ((volts - 3) / 1.2) * 100;

  if(perc < 0 || perc > 100){
    Serial.print("ERROR with perc: ");
    Serial.println(perc);
    perc = 50;
  }
  battery = perc;
  // Serial.print(output);          // debug value
  // Serial.print(" ");
  // Serial.print(volts);          // debug value
  // Serial.print(" ");
  // Serial.print(perc);          // debug value
  // Serial.print(" ");
  // Serial.println(ref);
  if(online){sendBatteryDataToAPI();}
}