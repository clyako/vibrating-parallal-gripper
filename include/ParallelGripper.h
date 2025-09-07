#ifndef ParallelGripper_h
#define ParallelGripper_h
#include <Arduino.h>
#include "Controller.h"
#include "Finger.h"
#include "ForceMotor.h"
#include "PinDefines.h"

#define CONTROL_LOOP_PIN 39
#define CONTROL_LOOP_FREQUENCY 40000

class ParallelGripper
{
public:
    ParallelGripper(float sampling_rate = CONTROL_LOOP_FREQUENCY);
    void initialize();
    void run();
    void stop();
    void move_to_center();
    void get_target_positions(float &_motor_1_left_target_position, float &_motor_2_left_target_position,
                              float &_motor_1_right_target_position, float &_motor_2_right_target_position);
    void drive_motors(float _motor_1_left_target_position, float _motor_2_left_target_position,
                      float _motor_1_right_target_position, float _motor_2_right_target_position);
    bool power_switch();
    void input_check(int num_times_to_flip_switch = 3);
    void encoder_check();

    ForceMotor force_motor;

    int translation_grip_force = 0;
    int rotation_grip_force = 0;
    bool plotting = false;
    bool run_device = false;
    bool mode_is_translation = true;
    bool direction_is_forward = true;

private:
    Finger _right_finger;
    Finger _left_finger;
    UserInputs _inputs;
    QuaidWaveform _translation_waveform;
    QuaidWaveform _rotation_waveform;

    float _motor_1_left_target_position;
    float _motor_2_left_target_position;
    float _motor_1_right_target_position;
    float _motor_2_right_target_position;

    int _motor_1_left_direction;
    int _motor_1_left_pwm;
    int _motor_2_left_direction;
    int _motor_2_left_pwm;
    int _motor_1_right_direction;
    int _motor_1_right_pwm;
    int _motor_2_right_direction;
    int _motor_2_right_pwm;

    int _motor_1_left_multiplier = 1;
    int _motor_2_left_multiplier = 1;
    int _motor_1_right_multiplier = 1;
    int _motor_2_right_multiplier = 1;

    // gripping motor relaxation
    elapsedMicros _relax_grip_timer;
    unsigned long _max_acceleration_start_time;
    unsigned long _max_acceleration_end_time;
    unsigned long _period;

    // for control loop
    elapsedMicros _control_loop_timer;
    unsigned long _control_loop_period = (int)(1e6 / ((float)CONTROL_LOOP_FREQUENCY)); // 25 µs (40 kHz)

    // values for plotting
    unsigned long _plot_period = 1000; // 1 ms
    elapsedMicros _plot_timer;

    // toggling digital pin for control loop frequency verification for verifying control loop speed
    void _toggle_pin();
    bool _control_pin_state = LOW;
};

#endif