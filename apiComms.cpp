#include "apiComms.h"
#include "Credentials.h"

// define default walking stick data
long code = 12345678;
int alert_fall = 30; //seconds
int alert_alarm = 30; //seconds
bool alerting = true;
int alert_volume = 100; //ms
bool lost = false;
int battery = 70;

bool heartLogging = false;
int nextHeartRateLog = 0;

bool emergency = true;

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
    Serial.print(".");
    attempt++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    // makeAPIRequest();
    // Serial.println(alert_fall);
    // Serial.println(alert_alarm);
    // Serial.println(alerting);
    // Serial.println(alert_volume);
    // Serial.println(lost);
    
    //sendAlertDataToAPI();
    sendEmergencyDataToAPI(false);

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
    //battery = doc["battery"];

    // Disconnect from the server
    client.stop();
    Serial.println("API request completed.");
  } else {
    Serial.println("Connection to API failed!");
  }
}


void sendHeartDataToAPI(int bpm) {
  // Connect to the API endpoint
  Serial.print("Connecting to API for PATCH request...");

  if (client.connect(apiEndpoint, 443)) {
    Serial.println("Connected!");

    // Create a JSON document
    const size_t JSON_DOC_SIZE = 200;
    StaticJsonDocument<JSON_DOC_SIZE> patchDoc;

    // Set the values for the JSON document
    patchDoc["heartRate"] = bpm;

    // Serialize the JSON document to a string
    String jsonBody;
    serializeJson(patchDoc, jsonBody);

    // Make the HTTPS PATCH request
    String request = "PATCH /StepSmart/api/alert?code=12345678 HTTP/1.1\r\n";
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
  Serial.print("Connecting to API for PATCH request...");

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
  Serial.print("Connecting to API for PATCH request...");

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
    String request = "PATCH /StepSmart/api/alert?code=12345678 HTTP/1.1\r\n";
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