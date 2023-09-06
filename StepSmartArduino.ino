#include <Simpletimer.h>

#include "apiComms.h"
#include "LEDsControl.h"
#include "buzzer.h"
#include "fallDetection.h"

const bool ONLINE = false;

long heartLogTime = 0;
const int fetchDelay = 10000;       // every 10s, update with data from cloud
const int sendDelay = 10000;       // every 10s, update with data from cloud
const int heartLogDelay = 60000;    // every minute take heart rate sample

Simpletimer fetchTimer{};
Simpletimer sendTimer{};
Simpletimer alert_fallTimer{};
Simpletimer alert_alarmTimer{};

bool deactivateEmergency = false;

// Interupts: 2, 3, 9, 10, 11, 13, A1, A5, A7
// Digiatl pins
const int ledSwitchPin = 2;
const int emergencyButtonPin = 3;
//const int ledControlPin = 5
// Analouge pins
const int batteryPin = 15;
const int heartPin = 16;

int alertStage = 0; // 0=off, 1=buzzer&lights, 2=notify

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  // Setup Pins
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledSwitchPin, INPUT_PULLUP);
  pinMode(emergencyButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(emergencyButtonPin), emergencySwitch, FALLING);
  attachInterrupt(digitalPinToInterrupt(ledSwitchPin), ledSwitch, CHANGE);

  pinMode(batteryPin, INPUT);
  pinMode(heartPin, INPUT);

  // Setup functions
  setupLeds();

  setupBuzzer();

  // Read battery level on start up
  showLedBatteryLevel(battery, 2000);

  // setupFallDetection();

  if(ONLINE == true){
    // Connect to Wi-Fi
    connectToWiFi();
  }

  // Setup timers
  fetchTimer.register_callback(fetchTimerCallback);
  sendTimer.register_callback(sendTimerCallback);
  alert_fallTimer.register_callback(alert_fallTimerCallback);
  alert_alarmTimer.register_callback(alert_alarmTimerCallback);
}


void loop() {
  const long loopTime = millis();  // Time since board started program in ms

  // if( loopTime + heartLogDelay > heartLogTime && heartLogging ){
  //   heartLogTime = loopTime + heartLogDelay;
  //   nextHeartRateLog = readHeatRate();
  //   if(nextHeartRateLog > 0){
  //     // send data to API
  //   }
  // }
  fetchTimer.run(fetchDelay);
  sendTimer.run(sendDelay);
  
  //if(!emergency){
    //detectFall();
 // }
 
  if(emergency == true){
    if(alertStage == 0){
      alert_fallTimer.run(alert_fall*1000);
    }else if(alertStage == 1){
      alert_alarmTimer.run(alert_alarm*1000);
    }else if(alertStage == 2){
      // Alerted and notified, so over
      emergency = false;
      alertStage = 0;
      showLedsOff();
      deactivaeBuzzer();
    }
  }

  buzzerLoop(loopTime);
}

// --- Timer functions --- //
void fetchTimerCallback(){
  makeAPIRequest();
}
void sendTimerCallback(){
  ;// ToDO
}
void alert_fallTimerCallback(){
  activateBuzzer();
  showRedLeds();
  alertStage = 1;
}
void alert_alarmTimerCallback(){
  if(alerting){
    // send data to api - send notifiactions
    Serial.println("Todo: send");
  }
  alertStage = 2;
}

// --- Other Functions --- //
bool emergencyButtonState = LOW;
void emergencySwitch(){
  emergencyButtonState = !emergencyButtonState;
  emergency = !emergency;
  alertStage = 0;
  delay(20);
}
bool ledSwitchState = LOW;
void ledSwitch(){
  ledSwitchState = !ledSwitchState;
  toggleLedTorch();
  delay(20);
}

int readHeatRate(){
  int sample = -1;
  // take heart rate sample

  return sample;
}

int readBatteryLevel(){
  int level = -1;
  // read battery level from analouge pin
  return level;
}