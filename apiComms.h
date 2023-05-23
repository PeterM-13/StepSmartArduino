#ifndef apiComms
#define apiComms

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiSSLClient.h>
#include <ArduinoJson.h>

// define stick data
extern int alert_fall;
extern long code;
extern int alert_alarm;
extern bool alerting;
extern int alert_volume;
extern bool lost;
extern int battery;

void connectToWiFi();
void makeAPIRequest();

#endif
