#include "project_14_mpu6050.h"
#include "project_14_neo_6m.h"
#include "mqtt_server.h"
#ifndef __WIRE_H__
#define __WIRE_H__
#endif

void setup_mqtt(void);
//void setup_mqtt();
void setup_wifi();
void callback(char* , byte* , unsigned int );
void reconnect();

void setup(void){
  uint32_t delayzz = 1000; // Delay between changing state on LED pin
  setup_mpu6050();
  setup_neo_6m();
  xTaskCreate(loop_mpu6050,"accelerometer",4032,NULL,2,NULL);
  setup_mqtt();
  xTaskCreate(loop_neo_6m,"GPS",4032,NULL,2,NULL);
  loop();
}

void loop(){
  while(1){
    if (!client.connected()) {
      reconnect();
    }

    client.loop();

    long now = millis();
    if (now - lastMsg > 5000) {
      lastMsg = now;

      if(accident==1){
        char lat[8], lon[8];
        dtostrf(latitude, 1, 2, lat);
        dtostrf(longitude, 1, 2, lon);
        client.publish("project_14/latitude", lat);
        client.publish("project_14/longitude", lon);
        Serial.print("publishing data at topic project/latitude as ");
        Serial.print(latitude);
        Serial.print("publishing data at topic project/longitude as ");
        Serial.print(longitude);

        client.publish("project_14/accident", accidentz); 
        Serial.print("publishing data at topic project/accident as Accident Detected!");
      }
      else{
        char accX_c[8], accY_c[8], accZ_c[8];
        // Convert the value to a char array
        dtostrf(accX, 1, 2, accX_c);
        dtostrf(accY, 1, 2, accY_c);
        dtostrf(accZ, 1, 2, accZ_c);
        //debugging
        Serial.print("publiishing at topic project_14_x ");
        Serial.print(accX_c);
        Serial.print("publiishing at topic project_14_y ");
        Serial.print(accX_c);
        Serial.print("publiishing at topic project_14_z ");
        Serial.print(accX_c);
        Serial.println("");
        //mqtt
        client.publish("project_14/x", accX_c);
        client.publish("project_14/y", accY_c);
        client.publish("project_14/z", accZ_c);
      }
    }
  }
}

void setup_mqtt(){
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  //debugging
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //wifi setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //debugging
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  //debugging
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