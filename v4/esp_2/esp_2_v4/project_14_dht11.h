#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int acPin = 23;

void setup_dht11() {
  Serial.begin(115200);
  dht.begin();
  pinMode(acPin, OUTPUT);
  digitalWrite(acPin, LOW);
}

void loop_dht11(void *pvParameters) {
  while(1){
    delay(2000);
  
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
  
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C, Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");
    
     if (temperature > 28) {
       digitalWrite(acPin, HIGH);
       Serial.println("AC turned on");
     } else {
       digitalWrite(acPin, LOW);
       Serial.println("AC turned off");
       }
    }
  }
}