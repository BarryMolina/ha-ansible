#pragma once
#include <functional>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

using MsgCallback = std::function<void(const char* topic, const char* payload)>;

class Connectivity {
public:
    void begin(MsgCallback cb);
    void loop();
    bool isConnected();
    bool publish(const char* topic, const char* payload);

private:
    void connectMqtt();

    WiFiClient   _wifiClient;
    PubSubClient _mqttClient;
    unsigned long _lastReconnectAttempt = 0;
};
