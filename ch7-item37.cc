#include <thread>
#include <vector>

bool conditionAreSatisfied() { return true; }
void performComputation(std::vector<int>& v) {}

constexpr auto tenMillion = 10000000;
constexpr auto tenMillion2 = 10'000'000; // c++14

bool doWork(std::function<bool(int)> filter,
            int maxVal = tenMillion)
{
  std::vector<int> goodVals;

  std::thread t([&filter, maxVal, &goodVals]
                {
                  for (auto i = 0; i <= maxVal; ++i) {
                    if (filter(i)) goodVals.push_back(i);
                  }
                });

  auto nh = t.native_handle();

  if (conditionAreSatisfied()) {
    t.join();
    performComputation(goodVals);
    return true;
  }

  return false;
}

class ThreadRAII {
public:
  enum class DtorAction { join, detach };

  ThreadRAII(std::thread&& t, DtorAction a)
  : action(a), t(std::move(t)) {}

  ~ThreadRAII()
  {
    if (t.joinable()) {
      if (action == DtorAction::join) {
        t.join();
      } else {
        t.detach();
      }
    }
  }

  std::thread& get() { return t; }

private:
  DtorAction action;
  std::thread t;
};

bool doWork2(std::function<bool(int)> filter,
             int maxVal = tenMillion)
{
  std::vector<int> goodVals;

  ThreadRAII t(
    std::thread([&filter, maxVal, &goodVals]
                {
                  for (auto i = 0; i <= maxVal; ++i) {
                    if (filter(i)) goodVals.push_back(i);
                  }
                }),
                ThreadRAII::DtorAction::join
  );

  auto nh = t.get().native_handle();

  if (conditionAreSatisfied()) {
    t.get().join();
    performComputation(goodVals);
    return true;
  }

  return false;
}

class ThreadRAII2 {
public:
  enum class DtorAction { join, detach };

  ThreadRAII2(std::thread&& t, DtorAction a)
  : action(a), t(std::move(t)) {}

  ~ThreadRAII2()
  {
    if (t.joinable()) {
      if (action == DtorAction::join) {
        t.join();
      } else {
        t.detach();
      }
    }
  }

  ThreadRAII2(ThreadRAII2&&) = default;
  ThreadRAII2& operator=(ThreadRAII2&&) = default;

  std::thread& get() { return t; }

private:
  DtorAction action;
  std::thread t;
};

bool f(int) { return true; }

int main()
{
  doWork(f);
  doWork2(f);

  return 0;
}
