#pragma once
#include <Arduino.h>

class RelayController {
public:
    void begin();
    bool turnOn();                       // returns false if compressor lock is active
    void turnOff();
    bool isOn() const;
    bool isLocked() const;
    unsigned long lockRemainingMs() const;

private:
    bool          _on            = false;
    bool          _everTurnedOff = false;
    unsigned long _lastOffTime   = 0;
};
