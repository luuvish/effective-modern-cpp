#include <array>

// c++11
constexpr int pow(int base, int exp) noexcept
{
  return (exp == 0 ? 1 : base * pow(base, exp - 1));
}

// c++14
constexpr int pow2(int base, int exp) noexcept
{
  auto result = 1;
  for (int i = 0; i < exp; ++i) result *= base;

  return result;
}

class Point {
public:
  constexpr Point(double xVal = 0, double yVal = 0) noexcept
  : x(xVal), y(yVal)
  {}

  constexpr double xValue() const noexcept { return x; }
  constexpr double yValue() const noexcept { return y; }

  void setX(double newX) noexcept { x = newX; }
  void setY(double newY) noexcept { y = newY; }

private:
  double x, y;
};

constexpr
Point midpoint(const Point& p1, const Point& p2) noexcept
{
  return { (p1.xValue() + p2.xValue()) / 2,
           (p1.yValue() + p2.yValue()) / 2 };
}

// c++14
class Point2 {
public:
  constexpr Point2(double xVal = 0, double yVal = 0) noexcept
  : x(xVal), y(yVal)
  {}

  constexpr double xValue() const noexcept { return x; }
  constexpr double yValue() const noexcept { return y; }

  constexpr void setX(double newX) noexcept { x = newX; } // c++14
  constexpr void setY(double newY) noexcept { y = newY; } // c++14

private:
  double x, y;
};

constexpr Point2 midpoint2(const Point2& p1, const Point2& p2) noexcept
{
  return { (p1.xValue() + p2.xValue()) / 2,
           (p1.yValue() + p2.yValue()) / 2 };
}

// c++14
constexpr Point2 reflection(const Point2& p) noexcept
{
  Point2 result;

  result.setX(-p.xValue());
  result.setY(-p.yValue());

  return result;
}

int main()
{
  {
    int sz;

    // constexpr auto arraySize1 = sz; error!
    // std::array<int, sz> data1; error!

    constexpr auto arraySize2 = 10;
    std::array<int, arraySize2> data2;
  }

  {
    int sz;
    const auto arraySize = sz;
    // std::array<int, arraySize> data; error!
  }

  {
    constexpr auto numConds = 5;
    std::array<int, pow(3, numConds)> results;
  }

  {
    auto base = 20, exp = 10;
    auto baseToExp = pow(base, exp);
  }

  {
    constexpr Point p1(9.4, 27.7);
    constexpr Point p2(28.8, 5.3);

    constexpr auto mid = midpoint(p1, p2);
  }

  {
    constexpr Point2 p1(9.4, 27.7);
    constexpr Point2 p2(28.8, 5.3);

    constexpr auto mid = midpoint2(p1, p2);

    constexpr auto reflectionMid = reflection(mid);
  }

  return 0;
}
