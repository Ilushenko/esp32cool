#include <Arduino.h>
#include "fan.h"

enum {
  PIN_FAN_POWER = 15,
  PIN_FAN_PWM = 5,
  PIN_FAN_TACHO = 4
};

FanCooler fan(PIN_FAN_POWER, PIN_FAN_TACHO, PIN_FAN_PWM);

void setup()
{
  Serial.begin(115200);
  fan.begin();
  delay(5000);
  Serial.printf("FAN started\n");
  delay(5000);
}

void loop()
{
  fan.coolNext();
  Serial.printf("FAN Speed %u\n", fan.speed());
  delay(8000);
  Serial.printf("FAN RPM: %u\n", fan.rpm());
  delay(1000);
  if (fan.speed() == FAN_SPEED0) delay(6000);
}
