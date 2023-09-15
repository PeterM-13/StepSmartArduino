#ifndef apiComms
#define apiComms

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiSSLClient.h>
#include <ArduinoJson.h>
#include "LEDsControl.h"

extern bool online;

// define stick data
extern int alert_fall;
extern long code;
extern int alert_alarm;
extern bool alerting;
extern int alert_volume;
extern bool lost;
extern int battery;
extern bool heartLogging;
extern bool emergency;

void connectToWiFi();
void makeAPIRequest();
void sendHeartDataToAPI(int bpm);
void sendEmergencyDataToAPI(bool fall);
void sendBatteryDataToAPI();
#endif
