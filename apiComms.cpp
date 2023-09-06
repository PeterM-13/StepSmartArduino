#include "apiComms.h"
#include "Credentials.h"

// define default walking stick data
long code = 12345678;
int alert_fall = 30; //seconds
int alert_alarm = 30; //seconds
bool alerting = false;
int alert_volume = 1000;
bool lost = false;
int battery = 70;

bool heartLogging = false;
int nextHeartRateLog = 0;

bool emergency = false;

const char* apiEndpoint = "stepsmartapi.onrender.com";

// Initialize the Wi-Fi client
WiFiSSLClient client;

void connectToWiFi() {
  int maxAttempts = 10;

  // Connect to Wi-Fi network
  Serial.print("Connecting to Wi-Fi...");
  
  WiFi.begin(ssid, pass);
  
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt <= maxAttempts) {
    delay(500);
    Serial.print(".");
    attempt++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi");
  }
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

    // Define the size of the JSON document
    const size_t JSON_DOC_SIZE = 384;

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
    alert_fall = atoi(alert["fall"]);
    alert_alarm = atoi(alert["alarm"]);
    alerting = (atoi(alert["alert"]) == 1);
    alert_volume = atoi(alert["volume"]);
    //const char* contacts_0 = doc["contacts"][0];
    //const char* contacts_1 = doc["contacts"][1];
    lost = doc["lost"];
    battery = doc["battery"];

    // Disconnect from the server
    client.stop();
    Serial.println("API request completed.");
  } else {
    Serial.println("Connection to API failed!");
  }
}