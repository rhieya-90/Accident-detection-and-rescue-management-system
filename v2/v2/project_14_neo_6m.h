
#include <SoftwareSerial.h>

SoftwareSerial ss(4, 3);

void setup_neo_6m(){
  //Serial.begin(9600);
  ss.begin(115200);
}

void loop_neo_6m(){
  while (ss.available() > 0){
    // get the byte data from the GPS
    byte gpsData = ss.read();
    Serial.write(gpsData);
  }
}
