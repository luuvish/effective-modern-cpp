#include <thread>
#include <future>

using namespace std::literals;

int doAsyncWork() noexcept
{
  return 0;
}

int main()
{
  std::thread t(doAsyncWork);
  t.join();

  auto fut = std::async(doAsyncWork);
  if (fut.wait_for(0s) != std::future_status::deferred) {
    while (fut.wait_for(100ms) != std::future_status::ready) {}
  }

  return 0;
}
