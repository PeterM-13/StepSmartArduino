#include "HeartMonitoring.h"

int heartSensorPin = A2; // potentiometer wiper (middle terminal) connected to analog pin 0

long total = 0;
int output = 0;
const int threshold = 518;
const int thresholdUpper = 550;
long time = 0;
long lastBeat = 0;
int prevOutput = 0;

const int numDataPoints =   199;
int data[numDataPoints];
int logIndex = 0;
int bpmAvrg[8];
int bpmAvrgIndex = 0;
bool logging = false;

void heartLoop(bool pulsing){
  const int val = analogRead(heartSensorPin);  // read the input pin
  total += val;
  output = int(total/100);
  total -= output;

  if(output > threshold && prevOutput <= threshold && logging == false){
    logging = true;
    logIndex = 0;
    //if(pulsing){showOrangeLeds();}
  }
  prevOutput = output;
  if(logging){
    if(output <= threshold){
      logging = false;
      time = millis();
      long beatTime = time - lastBeat;
      bool pass = true;
      if(beatTime < 2000 && beatTime > 400){ 
        if(pulsing){displayHeartBeat(3, 0, map(beatTime,400,2000,25,52));} //40
        for(int i=0; i<numDataPoints; i++){
          if(data[i] > thresholdUpper){
            pass = false;
            break;
          }
        }
        if(pass){
          //if(pulsing){displayHeartBeat(3, 0, 40);}
          int bpm = 60.0 / (beatTime/1000.0);
          //Serial.println("Beat!");
          if(bpm > 40 && bpm < 120){
            //Serial.print("BPM: ");
            //Serial.println(bpm);
            bpmAvrg[bpmAvrgIndex] = bpm;
            bpmAvrgIndex ++;
            if(bpmAvrgIndex > 7){
              bpmAvrgIndex = 0;
              int total = 0;
              for(int i=0; i<8; i++){
                total += bpmAvrg[i];
              }
              int avrgBpm = total / 8;
              //Serial.print("BPM = ");
              //Serial.println(avrgBpm);
              if(online){sendHeartDataToAPI(avrgBpm);}
            }
          }
        }
      }
      lastBeat = time;
    }else{
      data[logIndex] = output;
      logIndex ++;
      if(logIndex > numDataPoints-1){
        logging = false;
        logIndex = 0;
      }
    }
  }
}