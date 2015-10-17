#include <vector>
#include <mutex>
#include <cmath>

int main()
{
  {
    class Polynomial {
    public:
      using RootsType = std::vector<double>;

      RootsType roots() const
      {
        if (!rootsAreValid) {
          rootsAreValid = true;
        }
        return rootVals;
      }

    private:
      mutable bool rootsAreValid{ false };
      mutable RootsType rootVals{};
    };

    Polynomial p;

    auto rootsOfP = p.roots();
    auto valsGivingZero = p.roots();
  }

  {
    class Polynomial {
    public:
      using RootsType = std::vector<double>;

      RootsType roots() const
      {
        std::lock_guard<std::mutex> g(m);

        if (!rootsAreValid) {
          rootsAreValid = true;
        }
        return rootVals;
      }

    private:
      mutable std::mutex m;
      mutable bool rootsAreValid{ false };
      mutable RootsType rootVals{};
    };

    Polynomial p;

    auto rootsOfP = p.roots();
    auto valsGivingZero = p.roots();
  }

  {
    class Point {
    public:
      double distanceFromOrigin() const noexcept
      {
        ++callCount;
        return std::hypot(x, y);
      }

    private:
      mutable std::atomic<unsigned> callCount{ 0 };
      double x, y;
    };
  }

  {
    class Widget {
    public:
      int magicValue() const
      {
        if (cacheValid) return cachedValue;
        else {
          auto val1 = expensiveComputation1();
          auto val2 = expensiveComputation2();
          cachedValue = val1 + val2;
          cacheValid = true;
          return cachedValue;
        }
      }
      int expensiveComputation1() const { return 0; }
      int expensiveComputation2() const { return 1; }

    private:
      mutable std::atomic<bool> cacheValid{ false };
      mutable std::atomic<int> cachedValue;
    };
  }

  {
    class Widget {
    public:
      int magicValue() const
      {
        std::lock_guard<std::mutex> guard(m);

        if (cacheValid) return cachedValue;
        else {
          auto val1 = expensiveComputation1();
          auto val2 = expensiveComputation2();
          cachedValue = val1 + val2;
          cacheValid = true;
          return cachedValue;
        }
      }
      int expensiveComputation1() const { return 0; }
      int expensiveComputation2() const { return 1; }

    private:
      mutable std::mutex m;
      mutable int cachedValue;
      mutable bool cacheValid{ false };
    };
  }

  return 0;
}
