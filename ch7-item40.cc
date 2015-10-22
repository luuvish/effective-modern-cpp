#include <iostream>

int computeImportantValue() { return 0; }

int main()
{
  {
    std::atomic<int> ai(0);

    ai = 10;

    std::cout << ai;

    ++ai; // ai = 11
    --ai; // ai = 10
  }

  {
    volatile int vi(0);

    vi = 10;

    std::cout << vi;

    ++vi; // vi = 11
    --vi; // vi = 10
  }

  {
    std::atomic<bool> valAvailable(false);
    auto imptValue = computeImportantValue();
    valAvailable = true;
  }
  {
    volatile bool valAvailable(false);
    auto imptValue = computeImportantValue();
    valAvailable = true;
  }

  {
    int x = 10;

    auto y = x;
    y = x;
    x = 10;
    x = 20;
  }
  {
    int x = 10;

    auto y = x;
    x = 20;
  }

  {
    volatile int x = 10;

    auto y = x;
    y = x;
    x = 10;
    x = 20;
  }

  {
    std::atomic<int> x(10);

    std::atomic<int> y(x.load());
    y.store(x.load());
    x = 10;
    x = 20;
  }

  return 0;
}
