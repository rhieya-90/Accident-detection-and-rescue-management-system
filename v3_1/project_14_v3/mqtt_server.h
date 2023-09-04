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