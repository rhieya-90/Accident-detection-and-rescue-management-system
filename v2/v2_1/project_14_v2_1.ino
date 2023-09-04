#include "project_14_mpu6050.h"
#include "project_14_neo_6m.h"

#ifndef __WIRE_H__
#define __WIRE_H__
#endif

void setup(void){
    setup_mpu6050();
    setup_neo_6m();
    uint32_t delayzz = 1000; // Delay between changing state on LED pin
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
