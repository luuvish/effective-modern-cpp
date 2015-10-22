#include <vector>
#include <future>

std::vector<std::future<void>> futs;

class Widget {
public:

private:
  std::shared_future<double> fut;
};

int calcValue() { return 0; }

int main()
{
  {
    std::packaged_task<int()>
      pt(calcValue);

    auto fut = pt.get_future();
    std::thread t(std::move(pt));

    t.join();
  }

  return 0;
}
