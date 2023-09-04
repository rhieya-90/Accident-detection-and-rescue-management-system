#include "project_14_bh1750.h"
#include "project_14_hc_sr04.h"
#include "project_14_dht11.h"
#ifndef __WIRE_H__
#define __WIRE_H__
#endif

//void loop_bh1750(void *pvParameters);

TaskHandle_t loop_bh1750_h;

void setup() {
  uint32_t blink_delay = 1000;
  Serial.begin(115200);
  setup_bh1750();
  setup_hc_sr04();
  setup_dht11();
  xTaskCreate(loop_bh1750,"bh1750",2048,(void*) &blink_delay,2,NULL);
  xTaskCreate(loop_hc_sr04,"hc_sr04",2048,(void*) &blink_delay,2,NULL);
  xTaskCreate(loop_dht11,"dht11",2048,(void*) &blink_delay,2,NULL);
}

void loop(){

}