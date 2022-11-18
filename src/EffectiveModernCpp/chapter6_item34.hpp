#ifndef EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM34
#define EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM34

#include "../utilities/utilities.hpp"
#include <chrono>

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_34 {

// typedef for a point in time (see Item 9 for syntax)
using Time = std::chrono::steady_clock::time_point;

// see Item 10 for "enum class"
enum class Sound { Beep, Siren, Whistle };

// typedef for a length of time
using Duration = std::chrono::steady_clock::duration;

// at time t, make sound s for duration d
void setAlarm(Time t, Sound s, Duration d);

void test_alarm_lambda();

void test_alarm_std_bind();

void test_all();

} // namespace item_34

} // namespace chapter_6

} // namespace effective_mordern_cpp


#endif // EFFECTIVE_MODERN_CPP_CHAPTER6_ITEM34
