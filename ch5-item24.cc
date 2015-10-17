#include <string>
#include <vector>
#include <cassert>

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

template<typename T>
decltype(auto) pretty_name()
{
  return type_id_with_cvr<T>().pretty_name();
}

class Widget {
  int x, y;
};

std::string f1(Widget&& param) // param is a rvalue reference
{
  return pretty_name<decltype(param)>();
}

template<typename T>
std::string f2(std::vector<T>&& param) // param is a rvalue reference
{
  return pretty_name<decltype(param)>();
}

template<typename T>
std::string f3(T&& param) // param is an universal reference
{
  return pretty_name<decltype(param)>();
}

template<typename T>
std::string f4(const T&& param) // param is a rvalue reference
{
  return pretty_name<decltype(param)>();
}

template<class T, class Allocator = std::allocator<T>>
class vector1 {
public:
  void push_back(T&& x); // rvalue reference
};

template<class T, class Allocator = std::allocator<T>>
class vector2 {
public:
  template<class... Args>
  void emplace_back(Args&&... args); // universal reference
};

template<typename MyTemplateType>
void someFunc(MyTemplateType&& param);

int main()
{
  {
    Widget&& var1 = Widget(); // rvalue reference
    auto&& var2 = var1;       // universal reference
    auto&& var3 = std::move(var1);

    assert(f1(Widget()) == "Widget&&");
    assert(pretty_name<decltype(var1)>() == "Widget&&");

    assert(pretty_name<decltype(var2)>() == "Widget&");
    assert(pretty_name<decltype(var3)>() == "Widget&&");
  }

  {
    Widget w;
    assert(f3(w) == "Widget&");
    assert(f3(std::move(w)) == "Widget&&");
  }

  {
    std::vector<int> v;
    // f2(v); error!
    assert(f2(std::vector<int>(10)) ==
           "std::__1::vector<int, std::__1::allocator<int> >&&");
  }

  {
    Widget w;
    // f4(w); error!
    assert(f4(std::move(w)) == "Widget const&&");
  }

  {
    auto timeFuncInvocation =
      [](auto&& func, auto&&... params) {
        // timer start
        std::forward<decltype(func)>(func)(
          std::forward<decltype(params)>(params)...
          );
        // timer stop
      };
  }

  return 0;
}
