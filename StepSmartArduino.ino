#include <WiFiNINA.h>
#include <WiFiSSLClient.h>
#include "Credentials.h"

// Replace with your network credentials
//const char* ssid = "YourWiFiSSID";
//const char* password = "YourWiFiPassword";


const char* apiEndpoint = "stepsmartapi.onrender.com";

// Initialize the Wi-Fi client
WiFiSSLClient client;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  connectToWiFi();
}

void loop() {
  // Make the API request
  makeAPIRequest();
  
  // Wait for some time before making the next request
  delay(10000);
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
    while (client.available()) {
      String line = client.readStringUntil('\n');
      
      // Check if the chunk length line is encountered
      if (line.startsWith("HTTP/1.1 200 OK") || line.startsWith("HTTP/1.1 204 No Content")) {
        continue;
      }
      
      // Check if the final chunk is reached
      if (line == "0\r") {
        break;
      }
      
      // Print the response line
      Serial.println(line);
    }
    
    // Disconnect from the server
    client.stop();
    Serial.println("API request completed.");
  } else {
    Serial.println("Connection to API failed!");
  }
}