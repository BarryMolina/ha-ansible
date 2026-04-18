#include "sensor.h"
#include <Wire.h>
#include <config.h>

bool Sensor::begin() {
    Wire.begin(SENSOR_SDA, SENSOR_SCL);
    return _bme.begin(SENSOR_ADDR);
}

bool Sensor::read(float &tempF, float &humidity, float &pressurePsi) {
    float tempC  = _bme.readTemperature();
    float hum    = _bme.readHumidity();
    float presPa = _bme.readPressure();

    // Out-of-range values indicate an I2C failure or uninitialized sensor
    if (tempC  < -40.0f   || tempC  > 85.0f   ||
        hum    <   0.0f   || hum    > 100.0f  ||
        presPa < 30000.0f || presPa > 110000.0f) {
        return false;
    }

    tempF       = (tempC * 9.0f / 5.0f) + 32.0f;
    humidity    = hum / 100.0f;
    pressurePsi = presPa * 0.000145038f;
    return true;
}
