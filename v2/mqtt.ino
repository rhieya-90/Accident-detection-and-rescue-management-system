#include <WiFi.h>
#include <PubSubClient.h>

//sensor
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define ledPin 2

// mqtt
const char* ssid = "project14";
const char* password = "project14";
const char* mqtt_server = "10.42.0.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//sensor
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
float heart_rate=0;
float spo2=0;

// sensor detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);

  //sensor 
      Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
   pox.setOnBeatDetectedCallback(onBeatDetected);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // sensor
 pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        heart_rate=pox.getHeartRate();
        Serial.print(heart_rate);
       Serial.print("bpm / SpO2:");
       spo2=pox.getSpO2();
        Serial.print(spo2);
        Serial.println("%");
 tsLastReport = millis();
}


    
    // Convert the value to a char array 
    char rate[8];
    dtostrf(heart_rate, 1, 2, rate);
    Serial.print("heart_rate send to mqtt: ");
    Serial.println(rate);
    char spo22[8];
    dtostrf(spo2, 1, 2, spo22);
    Serial.print("spo2 sen to mqtt: ");
    Serial.println(spo2);


    float randomz=random()%100;
    Serial.print("random_num to mqtt: ");
      char randomzz[8];
     dtostrf(randomz, 1, 2, randomzz);
    Serial.println(randomz);
    
    //mqtt
    client.publish("project_14/heart_rate", rate);
    client.publish("project_14/spo2", spo22);
        client.publish("project_14/random_num", randomzz);

  }
}