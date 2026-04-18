#pragma once
#include <Adafruit_BME280.h>

class Sensor {
public:
    bool begin();
    bool read(float &tempF, float &humidity, float &pressurePsi);

private:
    Adafruit_BME280 _bme;
};
