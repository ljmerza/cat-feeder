
#ifndef COMMON_GUARD
#define COMMON_GUARD

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

extern PubSubClient client;

extern char* on_cmd;
extern char* off_cmd;

extern bool isOn;
extern int scoopsOfFood;

void turnOn();
void turnOff();

#endif