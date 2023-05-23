#include <WiFiNINA.h>
#include <WiFiSSLClient.h>
#include <ArduinoJson.h>
#include "Credentials.h"

const char* apiEndpoint = "stepsmartapi.onrender.com";

// Initialize the Wi-Fi client
WiFiSSLClient client;

// Define the size of the JSON document
const size_t JSON_DOC_SIZE = 384;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Connect to Wi-Fi
  connectToWiFi();
}

void loop() {
  // Make the API request
  makeAPIRequest();
  
  // Wait for some time before making the next request
  delay(12000);
}

void connectToWiFi() {
  // Connect to Wi-Fi network
  Serial.print("Connecting to Wi-Fi...");
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void makeAPIRequest() {
  // Connect to the API endpoint
  Serial.print("Connecting to API...");
  
  if (client.connect(apiEndpoint, 443)) {
    Serial.println("Connected!");
    
    // Make the HTTPS request
    client.print(String("GET /StepSmart/api?code=12345678 HTTP/1.1\r\n") +
                 "Host: " + apiEndpoint + "\r\n" +
                 "Connection: close\r\n\r\n");
    
    // Wait for the server's response
    while (client.connected()) {
      if (client.available()) {
        // Read the response from the server
        String line = client.readStringUntil('\n');
        
        // Check if the response body starts
        if (line == "\r") {
          Serial.println("API response:");
          break;
        }
      }
    }
    // Skip the chunked transfer encoding
    String response ;
    while (client.available()) {
     response = client.readStringUntil('\n');
      
      if (!isdigit(response.charAt(0))) {
        // Print the response line
        Serial.println(response);
        break;
      }
    }

    // Check if any data is available for parsing
    if (!client.available()) {
      Serial.println("No data received from the server");
      return;
    }

    // Create a JSON document
    StaticJsonDocument<JSON_DOC_SIZE> doc;

    // Parse the JSON response
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    // Retrieve data from the JSON document
    long code = doc["code"];
    
    JsonObject alert = doc["alert"];
    int alert_fall = atoi(alert["fall"]);
    int alert_alarm = atoi(alert["alarm"]);
    bool alerting = (atoi(alert["alert"]) == 1);
    int alert_volume = atoi(alert["volume"]);

    const char* contacts_0 = doc["contacts"][0];
    const char* contacts_1 = doc["contacts"][1];

    bool lost = doc["lost"];

    int battery = doc["battery"];

    // Disconnect from the server
    client.stop();
    Serial.println("API request completed.");
  } else {
    Serial.println("Connection to API failed!");
  }
}