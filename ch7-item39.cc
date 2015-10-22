#include <thread>
#include <future>
#include <vector>

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

  ThreadRAII(ThreadRAII&&) = default;
  ThreadRAII& operator=(ThreadRAII&&) = default;

  std::thread& get() { return t; }

private:
  DtorAction action;
  std::thread t;
};

void react() {}

std::promise<void> p;

void detect()
{
  std::thread t([]
                {
                  p.get_future().wait();
                  react();
                });

  p.set_value();

  t.join();
}

std::promise<void> p2;

void detect2()
{
  ThreadRAII tr(
    std::thread([]
                {
                  p2.get_future().wait();
                  react();
                }),
    ThreadRAII::DtorAction::join
  );

  p2.set_value();
}

constexpr int threadsToRun = 10;

std::promise<void> p3;

void detect3()
{
  auto sf = p3.get_future().share();

  std::vector<std::thread> vt;

  for (int i = 0; i < threadsToRun; ++i) {
    vt.emplace_back([sf]{ sf.wait();
                          react(); });
  }

  p3.set_value();

  for (auto& t : vt) {
    t.join();
  }
}

int main()
{
  {
    // std::condition_variable cv;
    // std::mutex m;
    //
    // cv.notify_one();
    //
    // std::unique_lock<std::mutex> lk(m);
    // cv.wait(lk);
  }

  {
    // std::condition_variable cv;
    // std::mutex m;
    //
    // bool flag(false);
    //
    // {
    //   std::lock_guard<std::mutex> g(m);
    //   flag = true;
    // }
    // cv.notify_one();
    //
    // {
    //   std::unique_lock<std::mutex> lk(m);
    //   cv.wait(lk, [] { return flag; });
    // }
  }

  {
    detect();
    detect2();
    detect3();
  }

  return 0;
}
