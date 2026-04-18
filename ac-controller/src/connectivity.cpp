#include "connectivity.h"
#include <config.h>
#include <secrets.h>

static MsgCallback s_userCallback;

static void mqttCallbackTrampoline(char *topic, byte *payload, unsigned int length)
{
    if (!s_userCallback)
        return;
    char msg[256];
    unsigned int len = length < sizeof(msg) - 1 ? length : sizeof(msg) - 1;
    memcpy(msg, payload, len);
    msg[len] = '\0';
    s_userCallback(topic, msg);
}

void Connectivity::begin(MsgCallback cb)
{
    s_userCallback = cb;
    _mqttClient.setClient(_wifiClient);
    _mqttClient.setServer(mqtt_server, MQTT_PORT);
    _mqttClient.setCallback(mqttCallbackTrampoline);

    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");

    connectMqtt();
    _lastReconnectAttempt = millis();
}

void Connectivity::loop()
{
    _mqttClient.loop();

    // if (WiFi.status() != WL_CONNECTED) {
    //     unsigned long now = millis();
    //     if (now - _lastReconnectAttempt >= RECONNECT_INTERVAL_MS) {
    //         _lastReconnectAttempt = now;
    //         Serial.println("WiFi disconnected, reconnecting...");
    //         WiFi.begin(ssid, wifi_password);
    //     }
    //     return;
    // }

    // if (!_mqttClient.connected()) {
    //     unsigned long now = millis();
    //     if (now - _lastReconnectAttempt >= RECONNECT_INTERVAL_MS) {
    //         _lastReconnectAttempt = now;
    //         connectMqtt();
    //     }
    // }
}

void Connectivity::connectMqtt()
{
    Serial.println("Connecting to MQTT...");
    if (_mqttClient.connect(MQTT_CLIENT_ID, mqtt_user, mqtt_password))
    {
        Serial.println("MQTT connected");
        _mqttClient.subscribe(TOPIC_SET_TEMP);
        _mqttClient.subscribe(TOPIC_CONTROL);
    }
    else
    {
        Serial.print("MQTT connect failed, rc=");
        Serial.println(_mqttClient.state());
    }
}

bool Connectivity::isConnected()
{
    return WiFi.status() == WL_CONNECTED && _mqttClient.connected();
}

bool Connectivity::publish(const char *topic, const char *payload)
{
    if (!_mqttClient.connected())
        return false;
    return _mqttClient.publish(topic, payload);
}
