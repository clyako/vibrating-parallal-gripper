#ifndef UserInputs_h
#define UserInputs_h
#include <Arduino.h>
#include <Bounce2.h>
#include "Enums.h"

#define POWER_ROCKER_PIN 13
#define MODE_ROCKER_PIN 14
#define DIRECTION_ROCKER_PIN 15
#define ROTATION_TYPE_ROCKER_PIN 16

class UserInputs
{
public:
    UserInputs();
    Bounce power_rocker;
    Bounce mode_rocker;
    Bounce direction_rocker;
    Bounce translation_type_rocker;
    void initialize();
    void update_switches();
    void update_power_switch();
    void update_mode_switch();
    void update_direction_switch();
    void update_translation_type_switch();

    bool run_device = false;
    Mode mode = Mode::Translation;
    Direction direction = Direction::Forward;
    TranslationType translation_type = TranslationType::InSync;

private:
    int _debounce_time = 20; // ms
};

#endif