#include <DHT.h>

#define DHTPIN 4          // Pin where the DHT11 sensor is connected
#define DHTTYPE DHT11     // Type of the DHT sensor 

DHT dht(DHTPIN, DHTTYPE);

const int acPin = 23;    // Pin connected to the AC relay or control

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(acPin, OUTPUT);
  digitalWrite(acPin, LOW); // Ensure AC is initially off
}

void loop() {
  delay(2000); // Delay for 2 seconds
  
  float temperature = dht.readTemperature(); // Read temperature in Celsius
  float humidity = dht.readHumidity();       // Read humidity
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    
    if (temperature > 28) {
      digitalWrite(acPin, HIGH); // Turn on AC
      Serial.println("AC turned on");
    } else {
      digitalWrite(acPin, LOW);  // Turn off AC
      Serial.println("AC turned off");
    }
  }
}
