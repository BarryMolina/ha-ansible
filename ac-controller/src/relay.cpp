#include "relay.h"
#include <config.h>

void RelayController::begin() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    _on = false;
}

bool RelayController::turnOn() {
    if (_on) return true;
    if (isLocked()) return false;
    digitalWrite(RELAY_PIN, HIGH);
    _on = true;
    return true;
}

void RelayController::turnOff() {
    if (!_on) return;
    digitalWrite(RELAY_PIN, LOW);
    _lastOffTime   = millis();
    _everTurnedOff = true;
    _on            = false;
}

bool RelayController::isOn() const {
    return _on;
}

bool RelayController::isLocked() const {
    if (!_everTurnedOff) return false;
    return (millis() - _lastOffTime) < MIN_OFF_MS;
}

unsigned long RelayController::lockRemainingMs() const {
    if (!isLocked()) return 0;
    return MIN_OFF_MS - (millis() - _lastOffTime);
}
