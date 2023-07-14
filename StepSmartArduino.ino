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
  // sclae to 'alert_volume' using PWM  (Don't worry about custom rythm pulsing)
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