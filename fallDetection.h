#include <Arduino_LSM6DS3.h>
#include "apiComms.h"
#include "LEDsControl.h"

void setupFallDetection();
bool detectFall();
bool detectLift();