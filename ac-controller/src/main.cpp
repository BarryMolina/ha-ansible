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
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <secrets.h>

// Temp sensor
Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
  char msg[length + 1];
  memcpy(msg, payload, length);
  msg[length] = '\0';
  Serial.print("Payload received: ");
  Serial.println(msg);
}

void setup()
{
  // So that we can write to the terminal
  Serial.begin(115200);

  // So that this doesn't run before our monitor is set up
  delay(3000);

  WiFi.begin(ssid, wifi_password);
  Serial.println("Attempting to connect to WIFI");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Wifi connected!");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

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

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Attempting reconnection...");
    if (client.connect("D1MiniClient", mqtt_user, mqtt_password))
    {
      Serial.println("Client connected!");
      client.subscribe("ac/setTemp");
    }
    else
    {
      Serial.println("Waiting a bit...");
      delay(5000);
    }
  }
}

bool setHigh = true;
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);

  double temp = (temp_event.temperature * 9.0 / 5.0) + 32.0;
  char tempStr[8];
  dtostrf(temp, 4, 2, tempStr);
  Serial.print(F("Temperature = "));
  Serial.print(temp);
  Serial.println(" *F");
  client.publish("ac/getTemp", tempStr);

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
