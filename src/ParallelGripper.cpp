#include "ParallelGripper.h"

ParallelGripper::ParallelGripper(float sampling_rate)
    : _right_finger(MOTOR_1_PWM_A, MOTOR_1_PWM_B, MOTOR_1_SLP,
                    MOTOR_2_PWM_A, MOTOR_2_PWM_B, MOTOR_2_SLP,
                    ENCODER_1_CHANNEL_NUM, ENCODER_1_CHANNEL_A, ENCODER_1_CHANNEL_B,
                    ENCODER_2_CHANNEL_NUM, ENCODER_2_CHANNEL_A, ENCODER_2_CHANNEL_B,
                    CONTROL_LOOP_FREQUENCY),
      _left_finger(MOTOR_3_PWM_A, MOTOR_3_PWM_B, MOTOR_3_SLP,
                   MOTOR_4_PWM_A, MOTOR_4_PWM_B, MOTOR_4_SLP,
                   ENCODER_3_CHANNEL_NUM, ENCODER_3_CHANNEL_A, ENCODER_3_CHANNEL_B,
                   ENCODER_4_CHANNEL_NUM, ENCODER_4_CHANNEL_A, ENCODER_4_CHANNEL_B,
                   CONTROL_LOOP_FREQUENCY),
      force_motor(),
      _translation_waveform(quaid_frequency_translation, min_acceleration_gs_translation, max_acceleration_gs_translation, sampling_rate),
      _rotation_waveform(quaid_frequency_rotation, min_acceleration_gs_rotation, max_acceleration_gs_rotation, sampling_rate) {
      };

void ParallelGripper::initialize()
{
    _inputs.initialize();
    _left_finger.initialize();
    _right_finger.initialize();
    force_motor.initialize(MOTOR_5_PWM_A, MOTOR_5_PWM_B, MOTOR_5_SLP);

    // _left_finger.set_motor_gains(2000, 4.0);
    // _right_finger.set_motor_gains(2000, 4.0);
    _left_finger.set_motor_gains(4000, 5.0);
    _right_finger.set_motor_gains(4000, 5.0);

    // parameters for grip motor relaxation
    // _max_acceleration_start_time = (unsigned long)(((float)_translation_waveform.max_acceleration_start_time) * 0.9);
    _max_acceleration_start_time = _translation_waveform.max_acceleration_start_time;
    _max_acceleration_end_time = _translation_waveform.max_acceleration_end_time;
    _period = _translation_waveform.period;

    _inputs.update_switches();

    run_device = _inputs.run_device;
    mode_is_translation = _inputs.mode == Mode::Translation;
    direction_is_forward = _inputs.direction == Direction::Forward;
}

void ParallelGripper::run()
{
    _inputs.update_power_switch();
    if (_inputs.run_device)
    {
        if (_control_loop_timer >= _control_loop_period)
        {
            _control_loop_timer = 0;
            get_target_positions(_motor_1_left_target_position, _motor_2_left_target_position,
                                 _motor_1_right_target_position, _motor_2_right_target_position);
            drive_motors(_motor_1_left_target_position, _motor_2_left_target_position,
                         _motor_1_right_target_position, _motor_2_right_target_position);

            if (mode_is_translation)
            {
                // if (!direction_is_forward)
                // {
                //     if (!_translation_waveform.position_index)
                //     {
                //         _relax_grip_timer = 0;
                //     }

                //     if (_relax_grip_timer > _max_acceleration_start_time && _relax_grip_timer <= _max_acceleration_end_time)
                //     {
                //         force_motor.drive_motor(0, translation_grip_force);
                //     }
                //     else if (_relax_grip_timer > _period)
                //     {
                //         _relax_grip_timer = 0;
                //     }
                //     else
                //     {
                //         force_motor.drive_motor(1, translation_grip_force);
                //     }
                // }
                // else
                // {
                //     force_motor.drive_motor(1, translation_grip_force);
                // }
                force_motor.drive_motor(1, translation_grip_force);
            }
            else
            {
                force_motor.drive_motor(1, rotation_grip_force);
            }
        }
    }
    else
    {
        stop();
        force_motor.stop_motor();
    }
}

void ParallelGripper::stop()
{
    _left_finger.stop();
    _right_finger.stop();
}

void ParallelGripper::get_target_positions(float &_motor_1_left_target_position, float &_motor_2_left_target_position,
                                           float &_motor_1_right_target_position, float &_motor_2_right_target_position)
{
    _inputs.update_direction_switch();
    _inputs.update_mode_switch();
    _inputs.update_translation_type_switch();
    // get quaid position
    mode_is_translation = _inputs.mode == Mode::Translation;
    direction_is_forward = _inputs.direction == Direction::Forward;
    float position = mode_is_translation ? _translation_waveform.get_waveform_data() : _rotation_waveform.get_waveform_data();
    // float position = _translation_waveform.get_waveform_data();
    // read two switches to see if rotation or translation and direction

    // Lookup table: {m1_left, m2_left, m1_right, m2_right}
    static constexpr int multipliers[][4] = {
        {1, 1, 1, 1},     // Translation, InSync, Forward
        {-1, -1, -1, -1}, // Translation, InSync, Reverse
        {1, 1, -1, -1},   // Translation, Out-of-plane, Forward
        {-1, -1, 1, 1},   // Translation, Out-of-plane, Reverse
        {1, -1, -1, 1},   // In-plane Rotation, Forward
        {-1, 1, 1, -1}    // In-plane Rotation, Reverse
    };

    // if (position_index == 0)
    // {
    //     if (_out_of_plane_counter >= _out_of_plane_cycles_before_correction + _number_of_correction_cycles - 1)
    //     {
    //         _out_of_plane_counter = 0;
    //     }
    //     else
    //     {
    //         _out_of_plane_counter++;
    //     }
    // }

    // Compute lookup index
    int index;
    if (mode_is_translation)
    {
        if (_inputs.translation_type == TranslationType::InSync)
        {
            index = (direction_is_forward) ? 0 : 1;
        }
        else
        {
            index = (direction_is_forward) ? 2 : 3;

            // if (_out_of_plane_cycles_before_correction && _out_of_plane_counter >= _out_of_plane_cycles_before_correction)
            // {
            //     index = 0;
            // }
        }
    }
    else // In-plane rotation
    {
        index = (direction_is_forward) ? 4 : 5;
    }

    // Compute target positions directly
    _motor_1_left_target_position = position * multipliers[index][0];
    _motor_2_left_target_position = position * multipliers[index][1];
    _motor_1_right_target_position = position * multipliers[index][2];
    _motor_2_right_target_position = position * multipliers[index][3];
}

void ParallelGripper::drive_motors(float _motor_1_left_target_position, float _motor_2_left_target_position,
                                   float _motor_1_right_target_position, float _motor_2_right_target_position)
{
    unsigned long timestamp = millis();

    float _motor_1_left_current_position = _left_finger.encoder_1.read_position();
    _left_finger.motor_1.compute_control_signal(_motor_1_left_direction, _motor_1_left_pwm, _motor_1_left_target_position, _motor_1_left_current_position);
    _left_finger.motor_1.drive_motor(_motor_1_left_direction, _motor_1_left_pwm);

    float _motor_2_left_current_position = _left_finger.encoder_2.read_position();
    _left_finger.motor_2.compute_control_signal(_motor_2_left_direction, _motor_2_left_pwm, _motor_2_left_target_position, _motor_2_left_current_position);
    _left_finger.motor_2.drive_motor(_motor_2_left_direction, _motor_2_left_pwm);

    float _motor_1_right_current_position = _right_finger.encoder_1.read_position();
    _right_finger.motor_1.compute_control_signal(_motor_1_right_direction, _motor_1_right_pwm, _motor_1_right_target_position, _motor_1_right_current_position);
    _right_finger.motor_1.drive_motor(_motor_1_right_direction, _motor_1_right_pwm);

    float _motor_2_right_current_position = _right_finger.encoder_2.read_position();
    _right_finger.motor_2.compute_control_signal(_motor_2_right_direction, _motor_2_right_pwm, _motor_2_right_target_position, _motor_2_right_current_position);
    _right_finger.motor_2.drive_motor(_motor_2_right_direction, _motor_2_right_pwm);

    if (plotting && _plot_timer > _plot_period) // add printing period here as well
    {
        _plot_timer = 0;

        Serial.write((byte *)&timestamp, 4);                       // Send 4 bytes
        Serial.write((byte *)&_motor_1_left_target_position, 4);   // Send 4 bytes
        Serial.write((byte *)&_motor_1_left_current_position, 4);  // Send 4 bytes
        Serial.write((byte *)&_motor_2_left_target_position, 4);   // Send 4 bytes
        Serial.write((byte *)&_motor_2_left_current_position, 4);  // Send 4 bytes
        Serial.write((byte *)&_motor_1_right_target_position, 4);  // Send 4 bytes
        Serial.write((byte *)&_motor_1_right_current_position, 4); // Send 4 bytes
        Serial.write((byte *)&_motor_2_right_target_position, 4);  // Send 4 bytes
        Serial.write((byte *)&_motor_2_right_current_position, 4); // Send 4 bytes
    }

    _toggle_pin();
}

bool ParallelGripper::power_switch()
{
    _inputs.update_power_switch();
    return _inputs.run_device;
}

void ParallelGripper::input_check(int num_times_to_flip_switch)
{
    auto check_switch = [&](const char *label, auto update_func, auto &current_state, auto print_state)
    {
        update_func();
        auto prev_state = current_state;
        Serial.print(label);
        Serial.print(": ");
        print_state(prev_state);

        int count = 0;
        while (count < num_times_to_flip_switch)
        {
            update_func(); // Directly call the lambda function
            if (current_state != prev_state)
            {
                prev_state = current_state;
                Serial.print(label);
                Serial.print(": ");
                print_state(prev_state);
                count++;
            }
        }
    };

    check_switch("Run Device", [&]()
                 { _inputs.update_power_switch(); }, _inputs.run_device, [](bool state)
                 { Serial.println(state ? "On" : "Off"); });

    check_switch("Mode", [&]()
                 { _inputs.update_mode_switch(); }, _inputs.mode, [](Mode state)
                 { Serial.println(state == Mode::Translation ? "Translation" : "Rotation"); });

    check_switch("Direction", [&]()
                 { _inputs.update_direction_switch(); }, _inputs.direction, [](Direction state)
                 { Serial.println(state == Direction::Forward ? "Forward" : "Backward"); });

    check_switch("Translation Type", [&]()
                 { _inputs.update_translation_type_switch(); }, _inputs.translation_type, [](TranslationType state)
                 { Serial.println(state == TranslationType::InSync ? "In Sync" : "Out of Sync"); });
}

void ParallelGripper::encoder_check()
{
    _left_finger.encoder_check(1, 2);
    _right_finger.encoder_check(3, 4);
}

void ParallelGripper::_toggle_pin()
{
    _control_pin_state = !_control_pin_state;
    digitalWrite(CONTROL_LOOP_PIN, _control_pin_state);
}