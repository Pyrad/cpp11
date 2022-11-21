#include "chapter6_item34.hpp"
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <functional>

namespace effective_mordern_cpp {

namespace chapter_6 {

namespace item_34 {

// at time t, make sound s for duration d
void setAlarm(Time t, Sound s, Duration d) {

    using namespace std::literals;

    const auto time_now = std::chrono::steady_clock::now();

    const auto time_diff = time_now - t;
    const auto tduration = std::chrono::duration_cast<std::chrono::seconds>(time_diff);
    std::cout << "time_diff: " << tduration.count() << "s"; // << std::endl;

    // const std::time_t t_c = std::chrono::system_clock::to_time_t(t - 1s);
    // const std::time_t t_c = std::chrono::steady_clock::to_time_t(t);
    // std::cout << "Time t is " << std::put_time(std::localtime(&t_c), "%F %T.\n");

    switch (s) {
        case Sound::Beep :
            fprintf(stdout, "Sound Beep\n");
            break;
        case Sound::Siren :
            fprintf(stdout, "Sound Siren\n");
            break;
        case Sound::Whistle :
            fprintf(stdout, "Sound Whistle\n");
            break;
        default:
            fprintf(stdout, "Sound Default\n");
            break;
    }
}

/**
 * An alarm lasts for 30 seconds, 1 hour later after it is set.
 * To show it's more readable using a lambda than std::bind
 * Using a lambda expression
 */
void test_alarm_lambda() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    // setSoundL ("L" for "lambda") is a function object allowing a sound to be specified
    // for a 30-sec alarm to go off an hour after it's set
    auto setSoundL = [](Sound s) {
        // make std::chrono components available w/o qualification
        using namespace std::chrono;
        setAlarm(steady_clock::now() + hours(1),    // alarm to go off
                    s,                              // in an hour for
                    seconds(30));                   // 30 seconds
    };

    setSoundL(Sound::Beep);
}

/**
 * An alarm lasts for 30 seconds, 1 hour later after it is set.
 * To show it's more readable using a lambda than std::bind
 * Using a std::bind object
 */
void test_alarm_std_bind() {
    using namespace std::literals;


    // Why here "std::chrono::steady_clock::now() + 1h" is not right?
    // --------------------------------------------------------------
    // Because it takes effect 1h later after std::bind is set, while
    // we'd like it to take effect 1h later after this bind object is called!
    auto setSoundB = std::bind(setAlarm, // "B" for "bind"
                                std::chrono::steady_clock::now() + 1h, // incorrect indeed!
                                std::placeholders::_1,
                                30s);

    setSoundB(Sound::Beep);
}

void test_all() {
    utilities::ShowStartEndMsg smsg(__FUNCTION__);

    test_alarm_lambda();

    test_alarm_std_bind();
}


} // namespace item_34

} // namespace chapter_6

} // namespace effective_mordern_cpp

