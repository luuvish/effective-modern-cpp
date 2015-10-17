#include <cassert>

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

template<typename T>
decltype(auto) pretty_name()
{
  return type_id_with_cvr<T>().pretty_name();
}

template<typename T>
std::string func(T&& param)
{
  return pretty_name<decltype(param)>();
}

class Widget {
  int x, y;
};

Widget widgetFactory()
{
  return Widget();
}

template<typename T>
void f(T&& fParam) {
  someFunc(std::forward<T>(fParam));
}

// c++11
template<typename T>
T&& forward(typename
              std::remove_reference<T>::type& param)
{
  return static_cast<T&&>(param);
}

// c++14
template<typename T>
T&& forward2(std::remove_reference_t<T>& param)
{
  return static_cast<T&&>(param);
}

template<typename T>
class Widget2 {
public:
  typedef T&& RvalueRefTo;
};

int main()
{
  {
    Widget w;
    assert(func(w) == "Widget&");
    assert(func(widgetFactory()) == "Widget&&");
  }

  {
    int x;
    // auto& & rx = x; error!

    // func(w);
    // void func(Widget& && param);
    // void func(Widget& param);
  }

  {
    // Widget& && forward(typename
    //                      std::remove_reference<Widget&>::type& param)
    // {
    //   return static_cast<Widget& &&>(param);
    // }
    // Widget& && forward(Widget& param)
    // {
    //   return static_cast<Widget& &&>(param);
    // }
    // Widget& forward(Widget& param)
    // {
    //   return static_cast<Widget&>(param);
    // }

    // Widget&& forward(typename
    //                    std::remove_reference<Widget>::type& param)
    // {
    //   return static_cast<Widget&&>(param);
    // }
    // Widget&& forward(Widget& param)
    // {
    //   return static_cast<Widget&&>(param);
    // }
  }

  {
    Widget w;

    auto&& w1 = w;
    // Widget& && w1 = w;
    // Widget& w1 = w; // w1 is lvalue reference

    auto&& w2 = widgetFactory();
    // Widget&& w2 = widgetFactory(); // w2 is rvalue reference
  }

  {
    Widget2<int&> w;
    // typedef int& && RvalueRefTo;
    // typedef int& RvalueRefTo;
    assert(pretty_name<Widget2<int&>::RvalueRefTo>() == "int&");
  }

  return 0;
}
