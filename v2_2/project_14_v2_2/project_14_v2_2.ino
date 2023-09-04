#include "project_14_mpu6050.h"
#include "project_14_neo_6m.h"
#include "mqtt_server.h"
#ifndef __WIRE_H__
#define __WIRE_H__
#endif



 #include <WiFi.h>
#include <PubSubClient.h>

#include <esp_wifi.h>

const char* ssid = "project14";
const char* password = "project14";
const char* mqtt_server = "10.42.0.1";
//long lastMsg = 0;

uint8_t newMACAddress[] = {0xf4, 0x96, 0x34, 0x9d, 0xe6, 0x2b};

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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
xTaskCreate(
    loop_neo_6m
    ,  "GPS" // A name just for humans
    ,  4032        // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,  NULL// (void*) &delayzz :Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,  2  // Priority
    ,  NULL // Task handle is not used here - simply pass NULL
    );
    
}

void loop(){
    Serial.print("Accelerometer range set to: ");
delay(1000);



}

void loop_mqtt(){
  while(1){
        if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // sensor
 //pox.update();

    // Convert the value to a char array 
  char accX_c[8], accY_c[8], accZ_c[8];
      dtostrf(accX, 1, 2, accX_c);

    
    //mqtt
    client.publish("project_14/x", accX_c);  

  }

  }
}






void setup_mqtt(){
// mqtt





  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //pinMode(ledPin, OUTPUT);

}

void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  
  Serial.print("[OLD] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  
  // ESP32 Board add-on before version < 1.0.5
  //esp_wifi_set_mac(ESP_IF_WIFI_STA, &newMACAddress[0]);
  
  // ESP32 Board add-on after version > 1.0.5
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  
  Serial.print("[NEW] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
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