#include <vector>
#include <functional>

using FilterContainer =
  std::vector<std::function<bool(int)>>;

FilterContainer filters;

int computeSomeValue1() { return 1; }
int computeSomeValue2() { return 2; }
int computeDivisor(int x, int y) { return 3; }

void addDivisorFilter()
{
  auto calc1 = computeSomeValue1();
  auto calc2 = computeSomeValue2();

  auto divisor = computeDivisor(calc1, calc2);

  filters.emplace_back(
    [&](int value) { return value % divisor == 0; } // divisor will dangle!
  );

  filters.emplace_back(
    [&divisor](int value)
    { return value % divisor == 0; } // divisor will dangle!
  );
}

template<typename C>
void workWithContainer(const C& container)
{
  auto calc1 = computeSomeValue1();
  auto calc2 = computeSomeValue2();

  auto divisor = computeDivisor(calc1, calc2);

  using ContElemT = typename C::value_type;

  using std::begin;
  using std::end;

  // c++11
  if (std::all_of(
        begin(container), end(container),
        [&](const ContElemT& value)
        { return value % divisor == 0; })) {}

  // c++14
  if (std::all_of(begin(container), end(container),
                  [&](const auto& value)
                  { return value % divisor == 0; })) {}

  filters.emplace_back(
    [=](int value) { return value % divisor == 0; }
  );
}

class Widget {
public:
  void addFilter() const;

private:
  int divisor;
};

void Widget::addFilter() const
{
  // filters.emplace_back(
  //   [](int value) { return value % divisor == 0; } // error!
  // );

  // filters.emplace_back(
  //   [divisor](int value) { return value % divisor == 0; } // error!
  // );

  filters.emplace_back(
    [=](int value) { return value % divisor == 0; }
  );

  auto currentObjectPtr = this;

  filters.emplace_back(
    [currentObjectPtr](int value)
    { return value % currentObjectPtr->divisor == 0; }
  );

  auto divisorCopy = divisor;

  filters.emplace_back(
    [divisorCopy](int value)
    { return value % divisorCopy == 0; }
  );

  filters.emplace_back(
    [=](int value)
    { return value % divisorCopy == 0; }
  );

  // c++14
  filters.emplace_back(
    [divisor = divisor](int value)
    { return value % divisor == 0; }
  );
}

void addDivisorFilter2()
{
  static auto calc1 = computeSomeValue1();
  static auto calc2 = computeSomeValue2();

  static auto divisor = computeDivisor(calc1, calc2);

  filters.emplace_back(
    [=](int value)
    { return value % divisor == 0; }
  );

  ++divisor; // modify divisor
}

int main()
{
  {
    filters.emplace_back(
      [](int value) { return value % 5 == 0; }
    );
  }

  return 0;
}
