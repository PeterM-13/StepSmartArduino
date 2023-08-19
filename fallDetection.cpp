#include  "fallDetection.h"

void setupFallDetection() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
  }

  Serial.println("detecting...");
}

void detectFall(){
  bool falling = false;
  bool fallen = false;

  float x, y, z;
  
  // gyro
  float threashold = 750.0;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
    if(x > threashold || x < threashold*-1.0){
      falling = true;
    }
    if(y > threashold || y < threashold*-1.0){
      falling = true;
    }
    if(z > threashold || z < threashold*-1.0){
      falling = true;
    }
  }

  if(falling){
    delay(1000);
  
    // accel
    threashold = 0.2;
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      if(x < threashold || x > threashold*-1.0){
        fallen = true;
      }
    }

    if(falling && fallen){
      Serial.println("Fall Detected!");
      emergency = true;
    }
  }
}
