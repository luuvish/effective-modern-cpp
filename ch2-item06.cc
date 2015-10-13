#include <cassert>
#include <vector>
#include <iostream>

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

class Matrix {
  int m[16];
public:
  Matrix& operator+(const Matrix& rhs) {
    return *this;
  }
};

std::vector<bool> features(const Widget& w)
{
  return std::vector<bool>{ false, false, false, false, false, true };
}

void processWidget(const Widget& w, bool highPriority)
{
}

int main()
{
  {
    Widget w;
    bool highPriority = features(w)[5];
    processWidget(w, highPriority);

    assert(highPriority == true);
  }

  {
    Widget w;
    auto highPriority = features(w)[5];

    assert(pretty_name<decltype(highPriority)>() ==
      "std::__1::__bit_reference<std::__1::vector<bool, std::__1::allocator<bool> >, true>");
  }

  {
    Widget w;
    Matrix m1, m2, m3, m4;
    std::vector<int> c;
    double d;

    auto highPriority = static_cast<bool>(features(w)[5]);
    auto sum = static_cast<Matrix>(m1 + m2 + m3 + m4);
    auto index = static_cast<int>(d * c.size());
  }

  return 0;
}
