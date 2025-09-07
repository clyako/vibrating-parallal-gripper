#ifndef ForceMotor_h
#define ForceMotor_h
#include <Arduino.h>
#include "Controller.h"
#include "PinDefines.h"

class ForceMotor : public Controller
{
public:
    ForceMotor();
    void force_control(float target_position, float current_position);
    float read_current();
    void movement_test(int movement_speed = 500);

private:
    // for driving motor
    int _PWM_A_PIN;
    int _PWM_B_PIN;
    int _max_speed = 300;

    float _current_slope = 0.4; // V / A
    float _counts_to_volts = 3.3 / 4095.0;
    float _zero_current_voltage = 2.5;

    // force control
    float _prev_error = 0;
    float _error_sum = 0;
    float _windup = 100;
    float _kp = 400;
    float _kd = 0;
    float _ki = 3.0;

    elapsedMillis _force_control_timer;
    uint32_t _force_control_period = 100;
};

#endif