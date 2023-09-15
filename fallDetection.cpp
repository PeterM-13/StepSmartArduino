#include  "fallDetection.h"

void setupFallDetection() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
  }

  Serial.println("detecting...");
}

bool detectFall(){
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
    Serial.println("Falling..?");
    // accel
    threashold = 0.3;
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      if(x > -0.3){
        fallen = true;
      }
    }

    if(falling && fallen){
      Serial.println("Fall Detected!");
      //emergency = true;
      return true;
    }
  }
  return false;
}


bool detectLift(){
  bool lifting = false;
  bool lifted = false;

  float x, y, z;
  
  // gyro
  float threashold = 200.0;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
    if(x > threashold || x < threashold*-1.0){
      lifting = true;
    }
    if(y > threashold || y < threashold*-1.0){
      lifting = true;
    }
    if(z > threashold || z < threashold*-1.0){
      lifting = true;
    }
  }

  if(lifting){
    delay(2000);
    Serial.println("lifting..?");
    // accel
    threashold = 0.2;
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      if(x < -0.8){
        lifted = true;
      }
    }

    if(lifting && lifted){
      Serial.println("Lift Detected!");
      //emergency = false;
      return true;
    }
  }
  return false;
}