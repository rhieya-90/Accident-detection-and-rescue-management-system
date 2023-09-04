#include <BH1750.h>
#include <Wire.h>
#define LED 2

BH1750 lightMeter(0x23);

void setup_bh1750() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(LED, OUTPUT);
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
}

void loop_bh1750(void *pvParameters) {
  while(1){
    float var = 320.00;
    if (lightMeter.measurementReady()) {
      float lux = lightMeter.readLightLevel();
      Serial.print("Light: ");
      Serial.print(lux);
      Serial.println(" lx");
      if(var <= lux){
        digitalWrite(LED, HIGH);
      } 
    }
  }
}