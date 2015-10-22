#include <chrono>
#include <functional>

using Time = std::chrono::steady_clock::time_point;

enum class Sound { Beep, Siren, Whistle };

using Duration = std::chrono::steady_clock::duration;

void setAlarm(Time t, Sound s, Duration d) {}

enum class Volume { Normal, Loud, LoudPlusPlus };

void setAlarm2(Time t, Sound s, Duration d) {}
void setAlarm2(Time t, Sound s, Duration d, Volume v) {}

enum class CompLevel { Low, Normal, High };

class Widget {};

Widget compress(const Widget& w, CompLevel lev) { return Widget(); }

class PolyWidget {
public:
  template<typename T>
  void operator()(const T& param) const {}
};

int main()
{
  { // c++11
    auto setSoundL =
      [](Sound s)
      {
        using namespace std::chrono;

        setAlarm(steady_clock::now() + hours(1),
                 s,
                 seconds(30));
      };
  }

  { // c++14
    auto setSoundL =
      [](Sound s)
      {
        using namespace std::chrono;
        using namespace std::literals;

        setAlarm(steady_clock::now() + 1h,
                 s,
                 30s);
      };
  }

  {
    using namespace std::chrono;
    using namespace std::literals;

    using namespace std::placeholders; // needed for use of "_1"

    auto setSoundB1 =
      std::bind(setAlarm,
                steady_clock::now() + 1h, // incorrect!
                _1,
                30s);

    // c++14
    auto setSoundB2 =
      std::bind(setAlarm,
                std::bind(std::plus<>(),
                          std::bind(steady_clock::now),
                          1h),
                _1,
                30s);

    // c++11
    auto setSoundB3 =
      std::bind(setAlarm,
                std::bind(std::plus<steady_clock::time_point>(),
                          std::bind(steady_clock::now),
                          hours(1)),
                _1,
                seconds(30));
  }

  {
    using namespace std::chrono;
    using namespace std::literals;

    using namespace std::placeholders; // needed for use of "_1"

    auto setSoundL =
      [](Sound s)
      {
        using namespace std::chrono;

        setAlarm(steady_clock::now() + 1h,
                 s,
                 30s);
      };

    // auto setSoundB =
    //   std::bind(setAlarm, // error!
    //             std::bind(std::plus<>(),
    //                       std::bind(steady_clock::now),
    //                       1h),
    //             _1,
    //             30s);

    using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);

    auto setSoundB =
      std::bind(static_cast<SetAlarm3ParamType>(setAlarm),
                std::bind(std::plus<>(),
                          std::bind(steady_clock::now),
                          1h),
                _1,
                30s);

    setSoundL(Sound::Siren);
    setSoundB(Sound::Siren);
  }

  {
    int lowVal, highVal;

    auto betweenL1 = // c++14
      [lowVal, highVal]
      (const auto& val)
      { return lowVal <= val && val <= highVal; };

    using namespace std::placeholders;

    auto betweenB1 = // c++14
      std::bind(std::logical_and<>(),
                std::bind(std::less_equal<>(), lowVal, _1),
                std::bind(std::less_equal<>(), _1, highVal));

    auto betweenB2 = // c++11
      std::bind(std::logical_and<bool>(),
                std::bind(std::less_equal<int>(), lowVal, _1),
                std::bind(std::less_equal<int>(), _1, highVal));

    auto betweenL2 = // c++11
      [lowVal, highVal]
      (int val)
      { return lowVal <= val && val <= highVal; };
  }

  {
    Widget w;

    using namespace std::placeholders;

    auto compressRateB = std::bind(compress, w, _1);

    auto compressRateL =
      [w](CompLevel lev)
      { return compress(w, lev); };

    compressRateL(CompLevel::High); // call by value
    compressRateB(CompLevel::High); // call by reference
  }

  {
    using namespace std::placeholders;

    PolyWidget pw;
    auto boundPW1 = std::bind(pw, _1);

    boundPW1(1930);
    boundPW1(nullptr);
    boundPW1("Rosebud");

    // c++14
    auto boundPW2 = [pw](const auto& param)
                    { pw(param); };

    boundPW2(1930);
    boundPW2(nullptr);
    boundPW2("Rosebud");
  }

  return 0;
}
