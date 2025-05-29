#include "fan.h"

volatile uint32_t FanCooler::_pulse = 0U;

FanCooler::FanCooler(const uint8_t power, const uint8_t tacho, const uint8_t pwm) :
    _power(power),
    _tacho(tacho),
    _pwm(pwm)
{
    // Configure digital pins
    pinMode(_power, OUTPUT);    // Via PNP
    pinMode(_tacho, INPUT);     // Pull-up Resistor
    // Setup channel of cooler PWM pin
    ledcSetup(FAN_CH, FAN_FREQ, FAN_RES);
    ledcAttachPin(_pwm, FAN_CH);
}

void FanCooler::begin()
{
    // Stop cooling at start
    digitalWrite(_power, LOW);
    ledcWrite(FAN_CH, 0U);
}

void FanCooler::coolDown(const fan_speed_t speed)
{
    if (speed == _speed) return;
    _speed = speed;
    digitalWrite(_power, FAN_SPEED0 == _speed ? LOW : HIGH);
    uint32_t duty = 0U;
    switch (_speed) {
    case FAN_SPEED0:
    case FAN_SPEED1:
        duty = 0U;
        break;
    case FAN_SPEED2:
        duty = 191U;
        break;
    case FAN_SPEED3:
        duty = 223U;
        break;
    case FAN_SPEED4:
        duty = FAN_MAX;
        break;
    }
    ledcWrite(FAN_CH, duty);
}

void FanCooler::coolNext()
{
    const fan_speed_t next = (FAN_SPEED4 == _speed) ? FAN_SPEED0 : static_cast<fan_speed_t>( _speed + 1U);
    coolDown(next);
}

uint32_t FanCooler::rpm() const
{
    if (FAN_SPEED0 == _speed) return 0U;
    _pulse = 0U;
    attachInterrupt(digitalPinToInterrupt(_tacho), [](){ ++_pulse; }, FALLING);
    delay(1000);
    detachInterrupt(digitalPinToInterrupt(_tacho));
    return _pulse * (60 / FAN_PPR);
}
