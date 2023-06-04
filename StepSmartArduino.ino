#include "apiComms.h"
#include "LEDsControl.h"

const int FETCH_DELAY_ms = 10000;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Connect to Wi-Fi
  connectToWiFi();

  // setup LED strip
  setupLeds();
  // Read battery level on start up
  showLedBatteryLevel(75);
  delay(1500);
  toggleLedTorch();
  
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  // Make the API request to update data
  makeAPIRequest();
  
  // Use data
  if(alerting){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Wait for some time before making the next request
  delay(FETCH_DELAY_ms);

  // if switch pressed, turn torch on:
  //toggleLedTorch();

}