#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "config.h"
#include "common.h"

WiFiClient wifiClient;
PubSubClient client(wifiClient);

char* on_cmd = "ON";
char* off_cmd = "OFF";

bool isOn = 0;

void turnOn(){
    isOn = 1;
    digitalWrite(MOTOR_PIN, HIGH);
}

void turnOff(){
    isOn = 0;
    digitalWrite(MOTOR_PIN, LOW);
}