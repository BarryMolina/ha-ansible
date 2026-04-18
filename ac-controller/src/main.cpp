/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor
  This example shows how to take Sensor Events instead of direct readings

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2652

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

void setup()
{
  // So that we can write to the terminal
  Serial.begin(115200);

  // So that this doesn't run before our monitor is set up
  delay(3000);

  // Set relay pin to output
  pinMode(D1, OUTPUT);

  Serial.println(F("BME280 Sensor event test"));

  // By default should be pins D1 and D2, actually connected to D3 and D4
  Wire.begin(D3, D4); // SDA, SCL

  // By default 0x77, "cheap" sensors set to 0x76 apparently
  if (!bme.begin(0x76))
  {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1)
      delay(10);
  }

  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();
}

bool setHigh = true;
void loop()
{
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);

  Serial.print(F("Temperature = "));
  Serial.print((temp_event.temperature * 9.0 / 5.0) + 32.0);
  Serial.println(" *F");

  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Humidity = "));
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure * 0.014503768078);
  Serial.println(" psi");

  if (setHigh)
  {
    // Turn relay ON
    Serial.println("D1 HIGH");
    // digitalWrite(D1, HIGH);
    setHigh = false;
  }
  else
  {
    // Turn relay OFF
    Serial.println("D1 LOW");
    // digitalWrite(D1, LOW);
    setHigh = true;
  }

  Serial.println();
  delay(5000);
}