
#ifndef CALLBACK_GUARD 
#define CALLBACK_GUARD

#include "Common.h"
#include <Arduino.h>

void recieveMqttMessage(char* topic, byte* payload, unsigned int length);
void sendMqttMessage(char* feeder_state);
bool processJson(char* message);

#endif
