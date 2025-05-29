#ifndef __FAN_H__
#define __FAN_H__

#include <Arduino.h>

/// @brief Fan cooler speed
/// @details Fan cooler have 4 speeds in this realisation
typedef enum : uint8_t {
    FAN_SPEED0 = 0, ///< Cooler off
    FAN_SPEED1,     ///< Speed #1
    FAN_SPEED2,     ///< Speed #2
    FAN_SPEED3,     ///< Speed #3
    FAN_SPEED4      ///< Speed #4
} fan_speed_t;

/// @class FanCooler fan.h
/// @brief Provide control of 4-wire PWM cooler
/// @details Features:
/// @li On/Off cooler via PNP transistor
/// @li Set speed
/// @li Measurement rotation speed in RPM
/// @li Indicate cooler speed by LED
class FanCooler {
    /// @brief Cooler PWM Parameters
    enum {
        FAN_CH = 0,                         ///< Channel
        FAN_PPR = 2,                        ///< Pulses per revolution
        FAN_RES = 8,                        ///< Resolution. \b Max = \b log2(80000000/FAN_FREQ)
        FAN_MAX = (1 << FAN_RES) - 1,       ///< Max duty cycle
        FAN_FREQ = 25000                    ///< Frequency
    };
    FanCooler() = delete;
    FanCooler(const FanCooler&) = delete;
    FanCooler& operator = (const FanCooler&) = delete;
public:
    /// @brief Constructor
    /// @param power Cooler power On/Off pin
    /// @param tacho Cooler tachometer pin
    /// @param pwm Cooler PWM pin
    explicit FanCooler(const uint8_t power, const uint8_t tacho, const uint8_t pwm);
    /// @brief Destructor
    ~FanCooler() {}
public:
    /// @brief Start cooler control
    void begin();
    /// @brief Cooling with one of 4 speed
    /// @details Stop cooling of \c FAN_SPEED0 speed
    /// @param speed Speed enumerator
    void coolDown(const fan_speed_t speed);
    /// @brief Set next speed
    /// @details After maximum speed - next speed is \c FAN_SPEED0 (stop)
    void coolNext();
    /// @brief Rotation speed getter
    /// @details Blocking method - \c 1000 ms
    /// @return Rotation speed in RPM
    uint32_t rpm() const;
    /// @brief Speed getter
    /// @return Speed enumerator
    inline fan_speed_t speed() const { return _speed; }
private:    
    const uint8_t _power;
    const uint8_t _tacho;
    const uint8_t _pwm;
    fan_speed_t _speed{FAN_SPEED0};
    static volatile uint32_t _pulse;
};

#endif // __FAN_H__
