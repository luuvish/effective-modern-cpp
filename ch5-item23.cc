#include <type_traits>
#include <string>
#include <chrono>
#include <cassert>

// c++11
template<typename T>
typename std::remove_reference<T>::type&&
move(T&& param)
{
  using ReturnType =
    typename std::remove_reference<T>::type&&;

  return static_cast<ReturnType>(param);
}

// c++14
template<typename T>
decltype(auto) move(T&& param)
{
  using ReturnType = std::remove_reference_t<T>&&;
  return static_cast<ReturnType>(param);
}

class Widget {
  int x, y;
};

void makeLogEntry(std::string, std::chrono::system_clock::time_point) {}
std::string process(const Widget& lvalArg) { return "process(const Widget&)"; }
std::string process(Widget&& rvalArg) { return "process(Widget&&)"; }

template<typename T>
std::string logAndProcess(T&& param)
{
  auto now = std::chrono::system_clock::now();

  makeLogEntry("Calling 'process'", now);
  return process(std::forward<T>(param));
}

class Widget2 {
public:
  Widget2(Widget2&& rhs)
  : s(std::move(rhs.s))
  { ++moveCtorCalls; }

private:
  static std::size_t moveCtorCalls;
  std::string s;
};

class Widget3 {
public:
  Widget3(Widget3&& rhs)
  : s(std::forward<std::string>(rhs.s)) // unconventional
  { ++moveCtorCalls; }

private:
  static std::size_t moveCtorCalls;
  std::string s;
};

int main()
{
  {
    class Annotation {
    public:
      explicit Annotation(std::string text);
    };
  }

  {
    class Annotation {
    public:
      explicit Annotation(const std::string text)
      : value(std::move(text))
      {}

    private:
      std::string value;
    };
  }

  {
    Widget w;

    assert(logAndProcess(w) == "process(const Widget&)");
    assert(logAndProcess(std::move(w)) == "process(Widget&&)");
  }

  return 0;
}
