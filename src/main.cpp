#include <Arduino.h>
#include "ParallelGripper.h"
#include "ForceMotor.h"
#include "PinDefines.h"

void log_object_position_and_force_data();

// #define ENCODER_TEST
// #define INPUT_TEST
#define FORCE_MOTOR_MOVEMENT_TEST
// #define FORCE_MOTOR_CALIBRATION
// #define PLOT
// #define LOG
uint8_t APPLY_FORCE = 0;
uint8_t NO_FORCE = 1;
uint8_t STATE = NO_FORCE;

int force_to_apply = 200;
int force_increment = 100;
int max_force = 1600;

ParallelGripper gripper = ParallelGripper();
int translation_grip_force = 0;
int rotation_grip_force = 0;

// stuff for logging data
elapsedMicros logging_timer;
unsigned long logging_period = 500; // log every 0.5 s

elapsedMillis print_timer;
unsigned long print_period = 500;

void setup()
{
  Serial.begin(115200);
  gripper.initialize();
  gripper.translation_grip_force = translation_grip_force;
  gripper.rotation_grip_force = rotation_grip_force;

#if defined(PLOT)
  gripper.plotting = true;
#endif

  // reset logging timer
  logging_timer = 0;
}

void loop()
{
#if defined(ENCODER_TEST)
  gripper.encoder_check();
#elif defined(INPUT_TEST)
  gripper.input_check();
#elif defined(FORCE_MOTOR_MOVEMENT_TEST)
  if (gripper.power_switch())
  {
    // gripper.force_motor.drive_motor(0, 100);
    // gripper.force_motor.read_current();
    gripper.force_motor.movement_test(600);
  }
#elif defined(FORCE_MOTOR_CALIBRATION)
  if (STATE == NO_FORCE)
  {
    if (gripper.power_switch() && force_to_apply <= max_force)
    {
      STATE = APPLY_FORCE;
      Serial.print("Drive Voltage (Counts): ");
      Serial.println(force_to_apply);
    }
    else
    {
      gripper.force_motor.drive_motor(0, 0);
    }
  }
  else if (STATE == APPLY_FORCE)
  {
    if (!gripper.power_switch())
    {
      force_to_apply += force_increment;
      STATE = NO_FORCE;
    }
    else
    {
      gripper.force_motor.drive_motor(1, force_to_apply);
      if (print_timer > print_period)
      {
        print_timer = 0;
      }
    }
  }

#else
  gripper.run();
  // if (gripper.power_switch())
  // {
  //   if (gripper.mode_is_translation)
  //   {
  //     // condition to catch when flipping the switch
  //     if (gripper.run_device && !prev_run_device)
  //     {
  //       relax_force_timer = 0;
  //       prev_run_device = true;
  //     }
  //     force_motor.drive_motor(1, 900);
  //   }
  //   else
  //   {
  //     force_motor.drive_motor(1, 1200 * 0);
  //   }
  // }
  // else
  // {
  //   force_motor.stop_motor();
  // }
#if defined(LOG)
  log_object_position_and_force_data();
#endif
#endif
}

void log_object_position_and_force_data()
{
  if (logging_timer > logging_period)
  {
    logging_timer = 0;

    // get object position and applied force
    unsigned long timestamp = micros();

    Serial.write((byte *)&timestamp, 4); // Send 4 bytes
  }
}