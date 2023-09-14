#include <Simpletimer.h>

#include "apiComms.h"
#include "LEDsControl.h"
#include "buzzer.h"
#include "fallDetection.h"
#include "HeartMonitoring.h"

const bool ONLINE = true;

long heartLogTime = 0;
const int fetchDelay = 100000;       // every 10s, update with data from cloud (remove 0 from each)
const int sendDelay = 200000;       // every 20s, update with data from cloud
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
const int batteryPin = A1;//15;

int alertStage = 0; // 0=off, 1=buzzer&lights, 2=notify

bool ledSwitchState = LOW;
bool emergencyButtonState = LOW;

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  while (!Serial) {
   ; // Wait for serial port to connect
  }
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup Pins
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledSwitchPin, INPUT_PULLUP);
  pinMode(emergencyButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(emergencyButtonPin), emergencySwitch, FALLING);
  attachInterrupt(digitalPinToInterrupt(ledSwitchPin), ledSwitch, CHANGE);

  // Setup functions
  setupLeds();

  setupBuzzer();

  // Read battery level on start up
  int percentage = readBatteryLevel();
  Serial.println(percentage);
  showLedBatteryLevel(percentage, 3000);

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
  heartLoop(!ledSwitchState);
}

// --- Timer functions --- //
void fetchTimerCallback(){
  Serial.println("Make API request");
  //makeAPIRequest();
}
void sendTimerCallback(){
  Serial.println("Send API request");
  // ToDO
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
void emergencySwitch(){
  emergencyButtonState = !emergencyButtonState;
  emergency = !emergency;
  if(emergency){
    alert_fallTimerCallback();
  }else{
    showLedsOff();
    deactivaeBuzzer();
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
int readBatteryLevel(){
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
  // Serial.print(output);          // debug value
  // Serial.print(" ");
  // Serial.print(volts);          // debug value
  // Serial.print(" ");
  // Serial.print(perc);          // debug value
  // Serial.print(" ");
  // Serial.println(ref);
  return perc;
}