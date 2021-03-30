
#include "config.h"
#include "common.h"
#include "callback.h"
#include "wifi_common.h"

int switchIsOff;
int keepTrackOfScoops = 0;
int currentScoopsOfFood = 0;
int turnOffMotor = 0;

void setup() {
    Serial.begin(9600);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(SWITCH_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    setup_wifi();
    setupOTA();

    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(recieveMqttMessage);
}

void loop() {
    // make sure connected to wifi and MQTT for status LED - do this FIRST
    if (client.connected() && WiFi.status() == WL_CONNECTED) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Attempting WIFI connection...");
        setup_wifi();
    }

    if (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        reconnect_mqtt();
    }

    // Process MQTT tasks
    client.loop();

    // Handle OTA
    ArduinoOTA.handle();

    delay(100);
    
    // if motor is on then keep track of switch toggles
    if (isOn){
        switchIsOff = digitalRead(SWITCH_PIN);

        // if we want to turn motor off then reset bool and turn off. also reset number of scoops for next time
        if (turnOffMotor){
            turnOffMotor = 0;
            currentScoopsOfFood = 0;
            turnOff();

        } else if (switchIsOff && currentScoopsOfFood >= scoopsOfFood){
            // if we met the number of scoops wanted then trigger to turn off motor
            turnOffMotor = 1;

        } else if (!switchIsOff) {
            // if switch just got turned on then we want to know w hen it turns back off to increment scoop counter
            keepTrackOfScoops = 1;

        } else if (keepTrackOfScoops) {
            // if we are coming from on to off on switch then increment scoop counter and reset on to off flag
            keepTrackOfScoops = 0;
            currentScoopsOfFood = currentScoopsOfFood + 1;
        }
    }
}