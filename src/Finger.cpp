#include "Finger.h"

Finger::Finger(int motor_1_pwm_A, int motor_1_pwm_B, int motor_1_slp,
               int motor_2_pwm_A, int motor_2_pwm_B, int motor_2_slp,
               int encoder_1_channel_num, int encoder_1_channel_A, int encoder_1_channel_B,
               int encoder_2_channel_num, int encoder_2_channel_A, int encoder_2_channel_B,
               unsigned long control_loop_frequency)
    : motor_1(),
      motor_2(),
      encoder_1(encoder_1_channel_num, encoder_1_channel_A, encoder_1_channel_B),
      encoder_2(encoder_2_channel_num, encoder_2_channel_A, encoder_2_channel_B)
{
    _motor_1_pwm_A = motor_1_pwm_A;
    _motor_1_pwm_B = motor_1_pwm_B;
    _motor_2_pwm_A = motor_2_pwm_A;
    _motor_2_pwm_B = motor_2_pwm_B;

    _motor_1_slp = motor_1_slp;
    _motor_2_slp = motor_2_slp;

    _control_loop_period = (int)(1e6 / ((float)control_loop_frequency));
};

void Finger::initialize()
{
    motor_1.initialize(_motor_1_pwm_A, _motor_1_pwm_B, _motor_1_slp);
    motor_2.initialize(_motor_2_pwm_A, _motor_2_pwm_B, _motor_2_slp);

    encoder_1.initialize(FILTER_COUNT, FILTER_SAMPLE_PERIOD, -1.0f);
    encoder_2.initialize(FILTER_COUNT, FILTER_SAMPLE_PERIOD, 1.0f);

    // calibrate encoders
    motor_1.go_to_end_stop();
    motor_2.go_to_end_stop();

    delay(1000);

    encoder_1.calibrate();
    encoder_2.calibrate();

    delay(100);

    // set gains
    motor_1.set_gains(1000, 2.0, (float)_control_loop_period);
    motor_2.set_gains(1000, 2.0, (float)_control_loop_period);
    // motor_1.set_gains(4000, 5.0, (float)_control_loop_period);
    // motor_2.set_gains(4000, 5.0, (float)_control_loop_period);

    motor_1.stop_motor();
    motor_2.stop_motor();
}

void Finger::stop()
{
    motor_1.stop_motor();
    motor_2.stop_motor();
}

void Finger::set_motor_gains(float kp, float kd)
{
    motor_1.set_gains(kp, kd, (float)_control_loop_period); // right motor
    motor_2.set_gains(kp, kd, (float)_control_loop_period); // left motor
}

void Finger::encoder_check(int first_encoder_num, int second_encoder_num)
{
    encoder_1.print_position_change(first_encoder_num);
    encoder_2.print_position_change(second_encoder_num);
}