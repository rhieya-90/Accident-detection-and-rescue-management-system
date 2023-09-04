#include "project_14_mpu6050.h"
#include "project_14_neo_6m.h"
#include "mqtt_server.h"
#ifndef __WIRE_H__
#define __WIRE_H__
#endif




#include <WiFi.h>
#include <PubSubClient.h>

//#include <esp_wifi.h>

const char* ssid = "project14";
const char* password = "project14";
const char* mqtt_server = "10.42.0.1";

//uint8_t newMACAddress[] = {0xf4, 0x96, 0x34, 0x9d, 0xe6, 0x2b};

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char accidentz[20]="Accident Detected!";

void setup_mqtt(void);
void setup_mqtt();
void setup_wifi();
void callback(char* , byte* , unsigned int );
void reconnect();


void setup(void){
      uint32_t delayzz = 1000; // Delay between changing state on LED pin
    setup_mpu6050();
    setup_neo_6m();
    setup_mqtt();
  xTaskCreate(
    loop_mpu6050
    ,  "accelerometer" // A name just for humans
    ,  4032        // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,  NULL// (void*) &delayzz :Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,  2  // Priority
    ,  NULL // Task handle is not used here - simply pass NULL
    );
    /*
xTaskCreate(
    loop_neo_6m
    ,  "GPS" // A name just for humans
    ,  4032        // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,  NULL// (void*) &delayzz :Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,  2  // Priority
    ,  NULL // Task handle is not used here - simply pass NULL
    );
    */
    
        loop();

}

void loop(){
  while(1){
    //Serial.print("Accelerometer range set to: ");
//delay(1000);
          //Serial.print("------------------loop");

if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;



    // Convert the value to a char array 
  char accX_c[8], accY_c[8], accZ_c[8];
      dtostrf(accX, 1, 2, accX_c);

    
    //mqtt
          Serial.print("publishing data");
        Serial.print(accX_c);

    client.publish("project_14/x", accX_c); 
    if(accident==1){
      client.publish("project_14/accident", accidentz); 
    }




}

}

}


void setup_mqtt(){
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //pinMode(ledPin, OUTPUT);

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

  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      //digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      //digitalWrite(ledPin, LOW);
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

void loop_mqtt_server() {
  while(1){
  
  }
  }
