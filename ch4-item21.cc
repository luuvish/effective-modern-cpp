#include <memory>
#include <vector>
#include <cassert>

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
  return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

class Widget {
  int x, y;
};

void processWidget(std::shared_ptr<Widget> spw, int priority) {}

int computePriority() { return 0; }

void cusDel(Widget *ptr) { delete ptr; }

int main()
{
  {
    auto upw1(std::make_unique<Widget>());
    std::unique_ptr<Widget> upw2(new Widget);

    auto spw1(std::make_shared<Widget>());
    std::shared_ptr<Widget> spw2(new Widget);
  }

  {
    processWidget(std::shared_ptr<Widget>(new Widget), // warning memory leak
                  computePriority());

    processWidget(std::make_shared<Widget>(),
                  computePriority());
  }

  {
    std::shared_ptr<Widget> spw(new Widget);
    auto spw2 = std::make_shared<Widget>();
  }

  {
    auto widgetDeleter = [](Widget* pw) { delete pw; };

    std::unique_ptr<Widget, decltype(widgetDeleter)>
      upw(new Widget, widgetDeleter);

    std::shared_ptr<Widget> spw(new Widget, widgetDeleter);
  }

  {
    auto upv = std::make_unique<std::vector<int>>(10, 20);
    auto spv = std::make_shared<std::vector<int>>(10, 20);

    assert(upv->size() == 10);
    assert(spv->size() == 10);    
  }

  {
    auto initList = { 10, 20 }; // create std::initializer_list
    auto spv = std::make_shared<std::vector<int>>(initList);

    assert(spv->size() == 2);
  }

  {
    class ReallyBigType {};

    auto pBigObj =
      std::make_shared<ReallyBigType>();

    std::shared_ptr<ReallyBigType> pBigObj2(new ReallyBigType);
  }

  {
    processWidget(
      std::shared_ptr<Widget>(new Widget, cusDel), // arg is rvalue
      computePriority()
    );

    std::shared_ptr<Widget> spw(new Widget, cusDel);
    processWidget(spw, computePriority()); // arg is lvalue

    processWidget(std::move(spw),
                  computePriority());
  }

  return 0;
}
