#include "ForceMotor.h"

ForceMotor::ForceMotor() : Controller() {};

void ForceMotor::force_control(float target_force, float current_force)
{
    if (_force_control_timer > _force_control_period)
    {
        _force_control_timer = 0;

        float error = (target_force - current_force);
        _error_sum += error;
        _error_sum = constrain(_error_sum, -_windup, _windup);
        float error_derivative = error - _prev_error;
        float control_signal = error * _kp + error_derivative * _kd + _error_sum * _ki;

        _prev_error = error;

        int direction = (control_signal > 0) - (control_signal < 0);
        int motor_pwm = constrain(abs(control_signal), 0, _max_speed);

        drive_motor(direction, motor_pwm);
    }
}

float ForceMotor::read_current()
{
    int current_read_counts = analogRead(CURRENT_SENSOR);
    float current_amps = (((float)current_read_counts) * _counts_to_volts - _zero_current_voltage) / _current_slope;

    return current_amps;
}

void ForceMotor::movement_test(int movement_speed)
{
    int32_t current_time = millis();
    while (millis() - current_time < 2000)
    {
        drive_motor(1, movement_speed);
        Serial.println(read_current());
    }

    current_time = millis();

    while (millis() - current_time < 2000)
    {
        drive_motor(0, movement_speed);
        Serial.println(read_current());
    }
    delay(2000);
}