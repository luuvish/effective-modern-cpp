#include <vector>

void f(const std::vector<int>& v) {}

template<typename... Ts>
void fwd(Ts&&... params) {
  f(std::forward<Ts>(params)...);
}

void f2(std::size_t val) {}

template<typename... Ts>
void fwd2(Ts&&... params) {
  f2(std::forward<Ts>(params)...);
}

class Widget {
public:
  static constexpr std::size_t MinVals = 28;
};

class Widget2 {
public:
  static constexpr std::size_t MinVals = 28;
};

constexpr std::size_t Widget2::MinVals;

// void f3(int (*pf)(int)) {}
void f3(int pf(int)) {}

template<typename... Ts>
void fwd3(Ts&&... params) {
  f3(std::forward<Ts>(params)...);
}

int processVal(int value) { return 1; }
int processVal(int value, int priority) { return 2; }

template<typename T>
T workOnVal(T param)
{ return param; }

int main()
{
  {
    // template<typename T>
    // void fwd(T&& param) {
    //   f(std::forward<T>(param));
    // }

    // template<typename... Ts>
    // void fwd(Ts&&... params) {
    //   f(std::forward<Ts>(params)...);
    // }
  }

  {
    f({ 1, 2, 3 });

    // fwd({ 1, 2, 3 }); error!

    auto il = { 1, 2, 3 }; // il's type deduced to be std::initializer_list<int>
    fwd(il);
  }

  {
    std::vector<int> widgetData;
    widgetData.reserve(Widget::MinVals);

    f2(Widget::MinVals); // f2(28);
    // fwd2(Widget::MinVals); link error!
    fwd2(Widget2::MinVals);
  }

  {
    f3(processVal);
    // fwd3(processVal); error!
    // fwd3(workOnVal); error!
  }

  {
    using ProcessFuncType = int (*)(int);
    ProcessFuncType processValPtr = processVal;

    fwd3(processValPtr);
    fwd3(static_cast<ProcessFuncType>(workOnVal));
  }

  {
    struct IPv4Header {
      std::uint32_t version:4,
                    IHL:4,
                    DSCP:6,
                    ECN:2,
                    totalLength:16;
    };

    IPv4Header h;
    f2(h.totalLength);
    // fwd2(h.totalLength); error!

    auto length = static_cast<std::uint16_t>(h.totalLength);
    fwd2(length);
  }

  return 0;
}
