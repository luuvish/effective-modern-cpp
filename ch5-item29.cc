#include <vector>
#include <array>

class Widget {
  int x, y;
};

int main()
{
  {
    std::vector<Widget> vw1;

    auto vw2 = std::move(vw1);
  }

  {
    std::array<Widget, 10000> aw1;

    auto aw2 = std::move(aw1);
  }

  return 0;
}
