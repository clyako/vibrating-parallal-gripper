#include <Arduino.h>

// #define FREQ_0_5_HZ_TRANSLATION
#define FREQ_2_HZ_TRANSLATION
// #define FREQ_5_HZ_TRANSLATION
// #define FREQ_10_HZ_TRANSLATION
// #define FREQ_15_HZ_TRANSLATION
// #define FREQ_20_HZ_TRANSLATION
// #define FREQ_30_HZ_TRANSLATION
// #define FREQ_40_HZ_TRANSLATION
// #define FREQ_60_HZ_TRANSLATION
// #define FREQ_70_HZ_TRANSLATION
// #define FREQ_80_HZ_TRANSLATION
// #define FREQ_100_HZ_TRANSLATION // DANGEROUS

// #define FREQ_0_5_HZ_ROTATION
#define FREQ_2_HZ_ROTATION
// #define FREQ_5_HZ_ROTATION
// #define FREQ_10_HZ_ROTATION
// #define FREQ_15_HZ_ROTATION
// #define FREQ_20_HZ_ROTATION
// #define FREQ_30_HZ_ROTATION
// #define FREQ_40_HZ_ROTATION
// #define FREQ_60_HZ_ROTATION
// #define FREQ_70_HZ_ROTATION
// #define FREQ_80_HZ_ROTATION
// #define FREQ_100_HZ_ROTATION // DANGEROUS

#if defined(FREQ_0_5_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 0.5;        // Hz
constexpr float min_acceleration_gs_translation = 0.0005; // gs (1 mm)
constexpr float max_acceleration_gs_translation = 0.01;   // gs

#elif defined(FREQ_2_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 2; // Hz
// constexpr float min_acceleration_gs_translation = 0.0085; // gs (1 mm)
constexpr float min_acceleration_gs_translation = 0.0120; // gs (3 mm)
constexpr float max_acceleration_gs_translation = 0.18;   // gs

#elif defined(FREQ_5_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 5;        // Hz
constexpr float min_acceleration_gs_translation = 0.05; // gs
constexpr float max_acceleration_gs_translation = 0.5;  // gs

#elif defined(FREQ_10_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 10; // Hz
// constexpr float min_acceleration_gs_translation = 0.08; // gs (1 mm)
constexpr float min_acceleration_gs_translation = 0.13; // gs (4 mm)
constexpr float max_acceleration_gs_translation = 20.0; // gs

#elif defined(FREQ_15_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 15; // Hz
// constexpr float min_acceleration_gs_translation = 0.08; // gs (1 mm)
constexpr float min_acceleration_gs_translation = 0.6;   // gs (3 mm) // 0.8
constexpr float max_acceleration_gs_translation = 100.0; // gs

#elif defined(FREQ_20_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 20;       // Hz
constexpr float min_acceleration_gs_translation = 1.0;  // gs
constexpr float max_acceleration_gs_translation = 20.0; // gs

#elif defined(FREQ_30_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 30; // Hz
// constexpr float min_acceleration_gs_translation = 0.4; // gs (1 mm)
constexpr float min_acceleration_gs_translation = 0.5;  // gs (3 mm)
constexpr float max_acceleration_gs_translation = 20.0; // gs

#elif defined(FREQ_40_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 40; // Hz
// constexpr float min_acceleration_gs_translation = 1.5; // gs
constexpr float min_acceleration_gs_translation = 2.0;  // gs
constexpr float max_acceleration_gs_translation = 20.0; // gs

#elif defined(FREQ_60_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 60; // Hz
// constexpr float min_acceleration_gs_translation = 2.5; // gs
constexpr float min_acceleration_gs_translation = 1.5;  // gs
constexpr float max_acceleration_gs_translation = 20.0; // gs

#elif defined(FREQ_70_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 70;         // Hz
constexpr float min_acceleration_gs_translation = 5.9767; // gs (1 mm)
// constexpr float min_acceleration_gs_translation = 5.0;   // gs (3mm)
constexpr float max_acceleration_gs_translation = 20.0; // gs

#elif defined(FREQ_80_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 80;       // Hz
constexpr float min_acceleration_gs_translation = 1.0;  // gs
constexpr float max_acceleration_gs_translation = 20.0; // gs

#elif defined(FREQ_100_HZ_TRANSLATION)
constexpr float quaid_frequency_translation = 80;        // Hz
constexpr float min_acceleration_gs_translation = 10.0;  // gs
constexpr float max_acceleration_gs_translation = 100.0; // gs

#endif

#if defined(FREQ_0_5_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 0.5;        // Hz
constexpr float min_acceleration_gs_rotation = 0.0005; // gs (1 mm)
constexpr float max_acceleration_gs_rotation = 0.05;   // gs

#elif defined(FREQ_2_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 2;          // Hz
constexpr float min_acceleration_gs_rotation = 0.0085; // gs (1 mm)
// constexpr float min_acceleration_gs_rotation = 0.0120; // gs (3 mm)
constexpr float max_acceleration_gs_rotation = 0.18; // gs

#elif defined(FREQ_5_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 5;        // Hz
constexpr float min_acceleration_gs_rotation = 0.05; // gs
constexpr float max_acceleration_gs_rotation = 0.5;  // gs

#elif defined(FREQ_10_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 10; // Hz
// constexpr float min_acceleration_gs_rotation = 0.08; // gs (1 mm)
constexpr float min_acceleration_gs_rotation = 0.2;  // gs (4 mm)
constexpr float max_acceleration_gs_rotation = 30.0; // gs

#elif defined(FREQ_15_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 15; // Hz
// constexpr float min_acceleration_gs_rotation = 0.08; // gs (1 mm)
constexpr float min_acceleration_gs_rotation = 0.6;  // gs (3 mm) // 0.8
constexpr float max_acceleration_gs_rotation = 10.0; // gs

#elif defined(FREQ_20_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 20;       // Hz
constexpr float min_acceleration_gs_rotation = 1.0;  // gs
constexpr float max_acceleration_gs_rotation = 20.0; // gs

#elif defined(FREQ_30_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 30;      // Hz
constexpr float min_acceleration_gs_rotation = 1.0; // gs (1 mm)
// constexpr float min_acceleration_gs_rotation = 2.5;  // gs (3 mm)
constexpr float max_acceleration_gs_rotation = 20.0; // gs

#elif defined(FREQ_40_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 40; // Hz
// constexpr float min_acceleration_gs_rotation = 1.5; // gs
constexpr float min_acceleration_gs_rotation = 1.0; // gs
constexpr float max_acceleration_gs_rotation = 5.0; // gs

#elif defined(FREQ_60_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 60; // Hz
// constexpr float min_acceleration_gs_rotation = 2.7; // gs (1 mm)
constexpr float min_acceleration_gs_rotation = 6.0;  // gs (3mm)
constexpr float max_acceleration_gs_rotation = 20.0; // gs

#elif defined(FREQ_70_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 70;         // Hz
constexpr float min_acceleration_gs_rotation = 5.9767; // gs (1 mm)
// constexpr float min_acceleration_gs_rotation = 5.0;   // gs (3mm)
constexpr float max_acceleration_gs_rotation = 20.0; // gs

#elif defined(FREQ_80_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 80;        // Hz
constexpr float min_acceleration_gs_rotation = 5.0;   // gs
constexpr float max_acceleration_gs_rotation = 250.0; // gs

#elif defined(FREQ_100_HZ_ROTATION)
constexpr float quaid_frequency_rotation = 80;        // Hz
constexpr float min_acceleration_gs_rotation = 10.0;  // gs
constexpr float max_acceleration_gs_rotation = 100.0; // gs

#endif
