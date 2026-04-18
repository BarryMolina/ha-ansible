#include <Arduino.h>
#include <ArduinoJson.h>
#include <math.h>
#include <config.h>
#include "sensor.h"
#include "relay.h"
#include "connectivity.h"

static Sensor          sensor;
static RelayController relay;
static Connectivity    connectivity;

static float targetTempF = NAN;
static bool  enabled     = false;

static void onMessage(const char* topic, const char* payload) {
    if (strcmp(topic, TOPIC_SET_TEMP) == 0) {
        float t = atof(payload);
        if (t >= SETPOINT_MIN_F && t <= SETPOINT_MAX_F) {
            targetTempF = t;
            Serial.printf("Setpoint: %.1f F\n", t);
        } else {
            Serial.printf("Ignoring out-of-range setpoint: %s\n", payload);
        }
    } else if (strcmp(topic, TOPIC_CONTROL) == 0) {
        if (strcasecmp(payload, "ON") == 0) {
            enabled = true;
            Serial.println("Controller enabled");
        } else if (strcasecmp(payload, "OFF") == 0) {
            enabled = false;
            relay.turnOff();
            Serial.println("Controller disabled");
        }
    }
}

static void runThermostat(float tempF) {
    if (!enabled || isnan(targetTempF)) return;

    if (tempF > targetTempF + HYSTERESIS_F) {
        if (!relay.turnOn()) {
            Serial.printf("AC on blocked — %lu s remaining\n",
                          relay.lockRemainingMs() / 1000);
        }
    } else if (tempF < targetTempF - HYSTERESIS_F) {
        relay.turnOff();
    }
}

static void publishSensor(float tempF, float humidity, float pressurePsi) {
    JsonDocument doc;
    doc["temperature_f"] = tempF;
    doc["humidity"]      = humidity;
    doc["pressure_psi"]  = pressurePsi;

    char buf[128];
    serializeJson(doc, buf, sizeof(buf));
    connectivity.publish(TOPIC_SENSOR, buf);
}

static void publishState(bool sensorOk) {
    JsonDocument doc;
    doc["enabled"]           = enabled;
    doc["relay_on"]          = relay.isOn();
    if (isnan(targetTempF)) {
        doc["target_temp_f"] = nullptr;
    } else {
        doc["target_temp_f"] = targetTempF;
    }
    doc["compressor_locked"] = relay.isLocked();
    doc["lock_remaining_s"]  = (unsigned int)(relay.lockRemainingMs() / 1000);
    doc["sensor_ok"]         = sensorOk;

    char buf[192];
    serializeJson(doc, buf, sizeof(buf));
    connectivity.publish(TOPIC_STATE, buf);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    relay.begin();

    if (!sensor.begin()) {
        Serial.println("FATAL: BME280 not found — check wiring!");
        while (true) delay(10);
    }
    Serial.println("BME280 ready");

    connectivity.begin(onMessage);
}

static unsigned long lastPublish = 0;

void loop() {
    connectivity.loop();

    unsigned long now = millis();
    if (now - lastPublish < PUBLISH_INTERVAL_MS) return;
    lastPublish = now;

    float tempF, humidity, pressurePsi;
    bool sensorOk = sensor.read(tempF, humidity, pressurePsi);

    if (!sensorOk) {
        Serial.println("Sensor read failed — relay off");
        relay.turnOff();
        publishState(false);
        return;
    }

    Serial.printf("Temp: %.1f F  Humidity: %.0f%%  Pressure: %.2f psi\n",
                  tempF, humidity * 100.0f, pressurePsi);

    publishSensor(tempF, humidity, pressurePsi);
    runThermostat(tempF);
    publishState(true);
}
