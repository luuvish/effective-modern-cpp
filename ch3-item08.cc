#include <cassert>
#include <string>
#include <mutex>

std::string f(int) { return "f(int)"; }
std::string f(bool) { return "f(bool)"; }
std::string f(void*) { return "f(void*)"; }

#undef NULL
#define NULL 0

void* findRecord() { return nullptr; }

class Widget {
  int x, y;
};

int    f1(std::shared_ptr<Widget> spw) { return 1; }
double f2(std::unique_ptr<Widget> upw) { return 0.0; }
bool   f3(Widget* pw) { return false; }

template<typename FuncType,
         typename MuxType,
         typename PtrType>
decltype(auto) lockAndCall(FuncType func,
                           MuxType& mutex,
                           PtrType ptr) {
  using MuxGuard = std::lock_guard<MuxType>;

  MuxGuard g(mutex);
  return func(ptr);
}

int main()
{
  {
    assert(f(0) == "f(int)");
    assert(f(NULL) == "f(int)");
    assert(f(nullptr) == "f(void*)");
  }

  {
    auto result = findRecord();
    if (result == 0) {}
    auto result1 = findRecord();
    if (result1 == nullptr) {}
  }

  {
    std::mutex f1m, f2m, f3m;

    using MuxGuard = std::lock_guard<std::mutex>;

    {
      MuxGuard g(f1m);
      auto result = f1(0);
    }
    {
      MuxGuard g(f2m);
      auto result = f2(NULL);
    }
    {
      MuxGuard g(f3m);
      auto result = f3(nullptr);
    }
  }

  {
    std::mutex f1m, f2m, f3m;

    // auto result1 = lockAndCall(f1, f1m, 0); error!
    // auto result2 = lockAndCall(f2, f2m, NULL); error!
    auto result3 = lockAndCall(f3, f3m, nullptr);
    assert(result3 == false);
  }

  return 0;
}
