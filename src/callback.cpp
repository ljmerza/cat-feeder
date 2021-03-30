#include <Arduino.h>
#include <ArduinoJson.h>

#include "common.h"
#include "callback.h"
#include "wifi_common.h"

const int BUFFER_SIZE = JSON_OBJECT_SIZE(10);
int scoopsOfFood = 0;

void recieveMqttMessage(char* topic, byte* payload, unsigned int length) {
    Serial.println(' ');
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    char message[length + 1];
    for (int i = 0; i < length; i++) {
        message[i] = (char)payload[i];
    }
    message[length] = '\0';
    Serial.println(message);

    if (!processJson(message)) {
        return;
    }
}

void sendMqttMessage(char* feeder_state) {
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& object = jsonBuffer.createObject();

    object["feeder_state"] = feeder_state;
    object["scoops_of_food"] = scoopsOfFood;

    char buffer[object.measureLength() + 1];
    client.publish(SET_TOPIC_SEND, buffer, true);
}

bool processJson(char* message) {
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(message);

    if (!root.success()) {
        Serial.println("parseObject() failed");
        return false;
    }

    // print the JSON to Serial
    root.prettyPrintTo(Serial);

    // are we on or off?
    if (root.containsKey("state")) {
        
        if (strcmp(root["state"], on_cmd) == 0) {
            if (root.containsKey("scoops")){
                scoopsOfFood = root["scoops"];
            } else {
                scoopsOfFood = 1;
            }

            // tell MQTT we are starting
            sendMqttMessage(on_cmd);

            turnOn();

        } else if (strcmp(root["state"], off_cmd) == 0) {
            turnOff();

            // tell MQTT we are ending
            sendMqttMessage(off_cmd);
        }
    }

    return true;
}

