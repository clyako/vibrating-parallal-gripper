#ifndef Finger_h
#define Finger_h
#include <Arduino.h>
#include "Controller.h"
#include "UserInputs.h"
#include "LinearEncoder.h"
#include "QuaidWaveform.h"
#include "QuaidWaveformSelector.h"

class Finger
{
public:
    Finger(int motor_1_pwm_A, int motor_1_pwm_B, int motor_1_slp,
           int motor_2_pwm_A, int motor_2_pwm_B, int motor_2_slp,
           int encoder_1_channel_num, int encoder_1_channel_A, int encoder_1_channel_B,
           int encoder_2_channel_num, int encoder_2_channel_A, int encoder_2_channel_B,
           unsigned long control_loop_frequency);
    void initialize();
    void stop();
    void set_motor_gains(float kp, float kd);
    void encoder_check(int first_encoder_num = 1, int second_encoder_num = 2);

    Controller motor_1, motor_2;
    LinearEncoder encoder_1, encoder_2;

    bool plotting = false;

private:
    int _motor_1_pwm_A;
    int _motor_1_pwm_B;
    int _motor_2_pwm_A;
    int _motor_2_pwm_B;

    int _motor_1_slp;
    int _motor_2_slp;

    // for setting derivative gain
    unsigned long _control_loop_period;
};

#endif