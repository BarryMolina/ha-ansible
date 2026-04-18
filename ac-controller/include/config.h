#pragma once

// Hardware pins
#define RELAY_PIN   D1
#define SENSOR_SDA  D3
#define SENSOR_SCL  D4  // also drives LED_BUILTIN — avoid LED use

// BME280 I2C address
#define SENSOR_ADDR 0x76

// Timing (milliseconds)
#define MIN_OFF_MS            (3UL * 60UL * 1000UL)
#define PUBLISH_INTERVAL_MS   5000UL
#define RECONNECT_INTERVAL_MS 5000UL

// Thermostat
#define HYSTERESIS_F   1.0f
#define SETPOINT_MIN_F 60.0f
#define SETPOINT_MAX_F 90.0f

// MQTT
#define MQTT_PORT      1883
#define MQTT_CLIENT_ID "ACController"
#define TOPIC_SET_TEMP "ac/setTemp"
#define TOPIC_CONTROL  "ac/control"
#define TOPIC_SENSOR   "ac/sensor"
#define TOPIC_STATE    "ac/state"
