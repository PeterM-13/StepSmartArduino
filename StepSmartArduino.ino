#include "apiComms.h"
#include "LEDsControl.h"


const bool ONLINE = false;

long fetchTime = 0;
long sendTime = 0;
long alert_fallTime = 0;
long alert_alarmTime = 0;
long heartLogTime = 0;
const int fetchDelay = 10000;       // every 10s, update with data from cloud
const int sendDelay = 10000;       // every 10s, update with data from cloud
const int heartLogDelay = 60000;    // every minute take heart rate sample

bool deactivateEmergency = false;

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  // Setup Pins
  pinMode(LED_BUILTIN, OUTPUT); // Pin for LED strip
  
  if(ONLINE == true){
    // Connect to Wi-Fi
    connectToWiFi();
  }

  // Setup LED strip
  setupLeds();
  // Read battery level on start up
  showLedBatteryLevel(75, 2000);
}


void loop() {
  const long loopTime = millis();  // Time since board started program in ms


  // Check light-swtich state
    // If High: 'switchLedsOn();'
    // 'elif(!emergency && loopTime > 1500): switchLedsOff();'


  // Check emergency button state
    // If high and !'emergency': 'emergency = true;'
    // If high and 'emergency': 'emergency = false;'

  if( loopTime + heartLogDelay > heartLogTime && heartLogging ){
    heartLogTime = loopTime;
    nextHeartRateLog = readHeatRate();
    if(nextHeartRateLog > 0){
      // send data to API
    }
  }
  

  if(detectFall() == true){
    emergency = true;
  }
  if(emergency == true){
    if( loopTime + alert_fallTime > alert_fall){
      activateBuzzer();
      showRedLeds();
    }
    if( loopTime + alert_alarmTime > alert_alarm && alerting == true ){
      // send API data
    }
  }else if(deactivateEmergency == true){
    switchLedsOff();
    deactivaeBuzzer();
  }
  deactivateEmergency = emergency;  // This is used to only turn lights off after emergecy - not while torch in use


  if( loopTime + fetchDelay > fetchTime && ONLINE == true ){
    fetchTime = loopTime;
    // Make the API request to update local data
    makeAPIRequest();
  }


  if( loopTime + sendDelay > sendTime && ONLINE == true ){
    sendTime = loopTime;
    // Make the API request to update local data
    makeAPIRequest();
  }
}



// Other Functions

void activateBuzzer(){

  // scale to 'alert_volume' using PWM  (Don't worry about custom rythm pulsing)

const int buzzer = 8; //buzzer to arduino pin 8  // the PWM pin the buzzer is attached to
int volume =  0;          // how loud the buzzer is
int fadeAmount = 25;     // how many points  to fade the buzzer volume by
int duty_cycle = 127;    // 0-255 so 50%  = 127 approx
int setting=0x01;

void setup(){
  
TCCR2B = TCCR2B & 0b11111000 | setting;//to  adjust divider for timer
  
// declare buzzer pin to be an output:
  pinMode(buzzer, OUTPUT);
}

void loop(){
  
  analogWrite (buzzer, volume);
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);         // ...for 1sec
 
  // set the volume of pin :
  analogWrite(buzzer,  volume);

  // change the volume for next time through the loop:
 volume  = volume + fadeAmount;

  // reverse the direction of the fading at the ends  of the fade:
 if (volume <= 0 || volume >= 255) {
    fadeAmount = -fadeAmount;
}
  // wait for 100 milliseconds to see the volume effect
  delay(100);
}



}
void deactivaeBuzzer(){

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

bool detectFall(){
  bool fallDetected = false;

  return fallDetected;
}

/* code for toggle switch if needed/if useful feel free to delete if not needed
#define LED_PIN 8
#define BUTTON_PIN 7
byte lastButtonState = LOW;
byte ledState = LOW;
unsigned long debounceDuration = 50; // millis
unsigned long lastTimeButtonStateChanged = 0;
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}
void loop() {
  if (millis() - lastTimeButtonStateChanged > debounceDuration) {
    byte buttonState = digitalRead(BUTTON_PIN);
    if (buttonState != lastButtonState) {
      lastTimeButtonStateChanged = millis();
      lastButtonState = buttonState;
      if (buttonState == LOW) {
        ledState = (ledState == HIGH) ? LOW: HIGH;
        digitalWrite(LED_PIN, ledState);
      }
    }
  }
}
*/