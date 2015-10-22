#include <thread>
#include <future>

using namespace std::literals;

void f()
{
  std::this_thread::sleep_for(1s);
}

// C++11
template<typename F, typename... Ts>
inline
std::future<typename std::result_of<F(Ts...)>::type>
reallyAsync(F&& f, Ts&&... params)
{
  return std::async(std::launch::async,
                    std::forward<F>(f),
                    std::forward<Ts>(params)...);
}

// c++14
template<typename F, typename... Ts>
inline
auto
reallyAsync2(F&& f, Ts&&... params)
{
  return std::async(std::launch::async,
                    std::forward<F>(f),
                    std::forward<Ts>(params)...);
}

int main()
{
  {
    auto fut1 = std::async(f);

    auto fut2 = std::async(std::launch::async |
                           std::launch::deferred,
                           f);
  }

  {
    auto fut = std::async(f);

    // while (fut.wait_for(100ms) !=
    //        std::future_status::ready) {
    // }
  }

  {
    auto fut = std::async(f);

    if (fut.wait_for(0s) ==
        std::future_status::deferred) {
      // wait or get
    } else {
      while (fut.wait_for(100ms) !=
             std::future_status::ready) {
      }
    }
  }

  {
    auto fut = std::async(std::launch::async, f);
  }

  {
    auto fut = reallyAsync(f);
    auto fut2 = reallyAsync2(f);
  }

  return 0;
}
