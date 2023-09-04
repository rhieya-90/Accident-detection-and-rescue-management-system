#include <TinyGPSPlus.h>
TinyGPSPlus gps;

float latitude,longitude;
void displayInfo();

void setup_neo_6m()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(3000);
}

void loop_neo_6m(void *pvParameters)
{
  Serial.println("GPS started");
  while(1){
    while (Serial2.available() > 0)
      if (gps.encode(Serial2.read()))
        displayInfo();

    if (millis() > 5000 && gps.charsProcessed() < 10){
      Serial.println(F("No GPS detected: check wiring."));
      while (true);
    }
  }
}

void displayInfo(){
  Serial.print(F("Location: "));
  if (gps.location.isValid()){
    /*
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("Lng: ");
    Serial.print(gps.location.lng(), 6);
    Serial.println();
    */
  latitude=gps.location.lat();
  longitude=gps.location.lng();
  }  
  else{
    Serial.print(F("Signal Weak!"));
    latitude=gps.location.lat();
  latitude=18.32;
  longitude=72.42;
  }
}
void updateSerial(){
  delay(500);
  while (Serial.available()){
    Serial2.write(Serial.read());
  }

  while (Serial2.available()){
    Serial.write(Serial2.read());
  }
}