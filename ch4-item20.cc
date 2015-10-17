#include <memory>
#include <unordered_map>
#include <string>
#include <cassert>

class Widget {
  int x, y;
};

using WidgetID = int;

std::unique_ptr<const Widget> loadWidget(WidgetID id)
{
  return std::make_unique<const Widget>();
}

std::shared_ptr<const Widget> fastLoadWidget(WidgetID id)
{
  static std::unordered_map<WidgetID,
                            std::weak_ptr<const Widget>> cache;

  auto objPtr = cache[id].lock();

  if (!objPtr) {
    objPtr = loadWidget(id);
    cache[id] = objPtr;
  }
  return objPtr;
}

int main()
{
  {
    auto spw = std::make_shared<Widget>();
    std::weak_ptr<Widget> wpw(spw);
    spw = nullptr;

    if (wpw.expired()) {}

    assert(wpw.expired() == true);
  }

  {
    auto spw = std::make_shared<Widget>();
    std::weak_ptr<Widget> wpw(spw);
    spw = nullptr;

    std::shared_ptr<Widget> spw1 = wpw.lock();
    auto spw2 = wpw.lock();

    std::string what = "";
    try {
      std::shared_ptr<Widget> spw3(wpw);
    } catch (std::bad_weak_ptr& e) {
      what = e.what();
    }
    assert(what == "bad_weak_ptr");
  }

  return 0;
}
