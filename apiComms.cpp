#include "apiComms.h"
#include "Credentials.h"

bool online = false;

// define default walking stick data
long code = 12345678;
int alert_fall = 30; //seconds
int alert_alarm = 30; //seconds
bool alerting = true;
int alert_volume = 100; //ms
bool lost = false;
int battery = 70;

bool heartLogging = true;

bool emergency = false;

const char* apiEndpoint = "stepsmartapi.onrender.com";

// Initialize the Wi-Fi client
WiFiSSLClient client;

void connectToWiFi() {
  const int maxAttempts = 10;

  // Connect to Wi-Fi network
  Serial.print("Connecting to Wi-Fi...");
  
  WiFi.begin(ssid, pass);
  
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt <= maxAttempts) {
    delay(500);
    //Serial.print(".");
    attempt++;
    noWifi(255, 140, 0);
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    online = true;
    noWifi(0, 255, 0);

  } else {
    Serial.println("\nFailed to connect to Wi-Fi");
    online = false;
    noWifi(255, 10, 0);
  }
}

void makeAPIRequest() {
  showOneLed(8, 3, 0);
  // Connect to the API endpoint
  Serial.print("Connecting to API for an update request...");
  client.setTimeout(4000);
  if (client.connect(apiEndpoint, 443)) {
    //Serial.println("Connected!");
    
    // Make the HTTPS request
    client.print("GET /StepSmart/api?code=12345678 HTTP/1.1\r\nHost: stepsmartapi.onrender.com\r\nConnection: close\r\n\r\n");
    
    // Wait for the server's response
    while (client.connected()) {
      if (client.available()) {
        // Read the response from the server
        String line = client.readStringUntil('\n');
        
        // Check if the response body starts
        if (line == "\r") {
          //Serial.println("API response:");
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
        //Serial.println(response);
        break;
      }
    }

    // Check if any data is available for parsing
    if (!client.available()) {
      Serial.println("No data received from the server");
      return;
    }

    // Define the size of the JSON document
    const size_t JSON_DOC_SIZE = 700;

    // Create a JSON document
    StaticJsonDocument<JSON_DOC_SIZE> doc;

    // Parse the JSON response
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    showOneLed(0, 100, 0);
    // Retrieve data from the JSON document
    long code = doc["code"];
    JsonObject alert = doc["alert"];
    alert_fall = atoi(alert["fall"]);
    alert_alarm = atoi(alert["alarm"]);
    alerting = (atoi(alert["alert"]) == 1);
    alert_volume = (11-atoi(alert["volume"])) * 20;
    lost = doc["lost"];
    JsonObject heartData = doc["heartrate"];
    heartLogging = (atoi("logging") == 1);

    // Disconnect from the server
    client.stop();
    Serial.println("API request completed.");
  } else {
    Serial.println("Connection to API failed!");
  }
}


void sendHeartDataToAPI(int bpm) {
  // Connect to the API endpoint
  Serial.print("Connecting to API for heart rate PATCH request...");

  if (client.connect(apiEndpoint, 443)) {
    Serial.println("Connected!");

    // Create a JSON document
    const size_t JSON_DOC_SIZE = 200;
    StaticJsonDocument<JSON_DOC_SIZE> patchDoc;

    // Set the values for the JSON document
    patchDoc["value"] = bpm;

    // Serialize the JSON document to a string
    String jsonBody;
    serializeJson(patchDoc, jsonBody);

    // Make the HTTPS PATCH request
    String request = "POST /StepSmart/api/heartBpm?code=12345678 HTTP/1.1\r\n";
    request += "Host: ";
    request += apiEndpoint;
    request += "\r\n";
    request += "Connection: close\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: ";
    request += String(jsonBody.length());
    request += "\r\n\r\n";
    request += jsonBody;

    client.print(request);
    client.setTimeout(4000);
    // Wait for the server's response
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("API response:");
          break;
        }
      }
    }
    // Read and print the response from the server
    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println(response);
    }
    // Disconnect from the server
    client.stop();
    Serial.println("PATCH request completed.");
  } else {
    Serial.println("Connection to API failed for PATCH request!");
  }
}


void sendEmergencyDataToAPI(bool fall) {
  // Connect to the API endpoint
  Serial.print("Connecting to API for emergency PATCH request...");

  if (client.connect(apiEndpoint, 443)) {
    Serial.println("Connected!");

    // Create a JSON document
    const size_t JSON_DOC_SIZE = 200;
    StaticJsonDocument<JSON_DOC_SIZE> patchDoc;

    // Set the values for the JSON document
    patchDoc["emergency"] = emergency;
    patchDoc["fall"] = fall;

    // Serialize the JSON document to a string
    String jsonBody;
    serializeJson(patchDoc, jsonBody);

    // Make the HTTPS PATCH request
    String request = "PATCH /StepSmart/api/emergency?code=12345678 HTTP/1.1\r\n";
    request += "Host: ";
    request += apiEndpoint;
    request += "\r\n";
    request += "Connection: close\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: ";
    request += String(jsonBody.length());
    request += "\r\n\r\n";
    request += jsonBody;

    client.print(request);
    client.setTimeout(4000);
    // Wait for the server's response
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("API response:");
          break;
        }
      }
    }
    // Read and print the response from the server
    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println(response);
    }
    // Disconnect from the server
    client.stop();
    Serial.println("PATCH request completed.");
  } else {
    Serial.println("Connection to API failed for PATCH request!");
  }
}


void sendBatteryDataToAPI() {
  // Connect to the API endpoint
  Serial.print("Connecting to API for battery PATCH request...");

  if (client.connect(apiEndpoint, 443)) {
    Serial.println("Connected!");

    // Create a JSON document
    const size_t JSON_DOC_SIZE = 200;
    StaticJsonDocument<JSON_DOC_SIZE> patchDoc;

    // Set the values for the JSON document
    patchDoc["battery"] = battery;

    // Serialize the JSON document to a string
    String jsonBody;
    serializeJson(patchDoc, jsonBody);

    // Make the HTTPS PATCH request
    String request = "PATCH /StepSmart/api/battery?code=12345678 HTTP/1.1\r\n";
    request += "Host: ";
    request += apiEndpoint;
    request += "\r\n";
    request += "Connection: close\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: ";
    request += String(jsonBody.length());
    request += "\r\n\r\n";
    request += jsonBody;

    client.print(request);
    client.setTimeout(4000);
    // Wait for the server's response
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("API response:");
          break;
        }
      }
    }
    // Read and print the response from the server
    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println(response);
    }
    // Disconnect from the server
    client.stop();
    Serial.println("PATCH request completed.");
  } else {
    Serial.println("Connection to API failed for PATCH request!");
  }
}