#include <cassert>
#include <vector>
#include <string>
#include <sstream>

class Widget {
public:
  Widget(int _x=0, int _y=0) : x(_x), y(_y) {}
protected:
  int x{ 0 };
  int y = 0;
  // int z(0); error!

public:
  std::string str() const {
    std::stringstream s;
    s << "(" << x << "," << y << ")";
    return s.str();
  }
};

class Widget2 : public Widget {
public:
  Widget2(int i, bool b) : Widget(i, b ? 1 : 0) {}
  Widget2(int i, double d) : Widget(i, d) {}
};

class Widget3 : public Widget {
public:
  Widget3(int i, bool b) : Widget(i, b ? 1 : 0) {}
  Widget3(int i, double d) : Widget(i, d) {}
  Widget3(std::initializer_list<long double> il) {
    auto it = il.begin();
    if (it != il.end()) x = *(it++) + 10;
    if (it != il.end()) y = *(it++) + 20;
  }
};

class Widget4 : public Widget {
public:
  Widget4(int i, bool b) : Widget(i, b ? 1 : 0) {}
  Widget4(int i, double d) : Widget(i, d) {}
  Widget4(std::initializer_list<long double> il) {
    auto it = il.begin();
    if (it != il.end()) x = *(it++) + 10;
    if (it != il.end()) y = *(it++) + 20;
  }
  operator float() const { return static_cast<float>(x + y - 3); }
};

class Widget5 : public Widget {
public:
  Widget5(int i, bool b) : Widget(i, b ? 1 : 0) {}
  Widget5(int i, double d) : Widget(i, d) {}
  Widget5(std::initializer_list<bool> il) {
    auto it = il.begin();
    if (it != il.end()) x = *(it++) + 10;
    if (it != il.end()) y = *(it++) + 20;
  }
};

class Widget6 : public Widget {
public:
  Widget6(int i, bool b) : Widget(i, b ? 1 : 0) {}
  Widget6(int i, double d) : Widget(i, d) {}
  Widget6(std::initializer_list<std::string> il) : Widget(1, 1) {}
};

class Widget7 : public Widget {
public:
  Widget7() : Widget(0, 0) {}
  Widget7(std::initializer_list<int> il) : Widget(1, 1) {}
};

int main()
{
  {
    int x(0);
    int y = 0;
    int z{ 0 };
    int z1 = { 0 };
  }

  {
    Widget w1;
    Widget w2 = w1;
    w1 = w2;
  }

  {
    std::vector<int> v{ 1, 3, 5 };
  }

  {
    std::atomic<int> ai1{ 0 };
    std::atomic<int> ai2(0);
    // std::atomic<int> ai3 = 0; error!
  }

  {
    double x, y, z;
    // int sum1{ x + y + z }; error!
    int sum2(x + y + z);
    int sum3 = x + y + z;
  }

  {
    Widget w1(10);
    // Widget w2(); declare function w2()
    Widget w3{};

    assert(w1.str() == "(10,0)");
    assert(w3.str() == "(0,0)");
  }

  {
    Widget2 w1(10, true);
    Widget2 w2{10, true};
    Widget2 w3(10, 5.0);
    Widget2 w4{10, 5.0};

    assert(w1.str() == "(10,1)");
    assert(w2.str() == "(10,1)");
    assert(w3.str() == "(10,5)");
    assert(w4.str() == "(10,5)");
  }

  {
    Widget3 w1(10, true);
    Widget3 w2{10, true};
    Widget3 w3(10, 5.0);
    Widget3 w4{10, 5.0};

    assert(w1.str() == "(10,1)");
    assert(w2.str() == "(20,21)");
    assert(w3.str() == "(10,5)");
    assert(w4.str() == "(20,25)");
  }

  {
    Widget4 w4{10, 5.0};
    Widget4 w5(w4);
    Widget4 w6{w4};

    Widget4 w7(std::move(w4));
    Widget4 w8{std::move(w4)};

    assert(w4.str() == "(20,25)");
    assert(w5.str() == "(20,25)");
    assert(w6.str() == "(20,25)"); // (52,0) ?

    assert(w7.str() == "(20,25)");
    assert(w8.str() == "(20,25)"); // (52,0) ?
  }

  {
    // Widget5 w{10, 5.0}; error! -Wc++11-narrowing
  }

  {
    Widget6 w1(10, true);
    Widget6 w2{10, true};
    Widget6 w3(10, 5.0);
    Widget6 w4{10, 5.0};

    assert(w1.str() == "(10,1)");
    assert(w2.str() == "(10,1)");
    assert(w3.str() == "(10,5)");
    assert(w4.str() == "(10,5)");
  }

  {
    Widget7 w1;
    Widget7 w2{};
    // Widget7 w3(); declare function w3()
    Widget7 w4({});
    Widget7 w5{{}};

    assert(w1.str() == "(0,0)");
    assert(w2.str() == "(0,0)");
    assert(w4.str() == "(1,1)");
    assert(w5.str() == "(1,1)");
  }

  {
    std::vector<int> v1(10, 20);
    std::vector<int> v2{10, 20};

    assert(v1.size() == 10 && v1[0] == 20 && v1[9] == 20);
    assert(v2.size() == 2 && v2[0] == 10 && v1[1] == 20);
  }

  return 0;
}
