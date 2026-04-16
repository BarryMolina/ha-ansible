#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(1000);                     // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  delay(100);                      // Wait for two seconds (to demonstrate the active low LED)
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}