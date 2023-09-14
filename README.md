# Step Smart Arduino Code

This code will run on the Arduino Nano 33 IoT, and act as the brain for the walking stick prototype.

## Dependencies
Install the ArduinoSAMD board to be able to upload sketches to the board.<br>
Install & include these Aduino libaries:
- `wifiNINA` (for the on-board wifi chip)
- `ArduinoLSM6DS3` (for the on-board Inertial Measurement Unit, IMU)
- `simpletimer` (for handling intervals)
- `ArduinoRTCZero` (for the on-board real-time clock, RTC)
- `ArduinoBLE` (for the on-board bluetooth chip)
- `ArduinoJSON` (for parsing json format)

## Environment variables
WiFi credentials:
- `ssid` (name of the wifi network)
- `pass` (password for network)

## Authors
- Peter Metcalfe
- Melissa Hobbs