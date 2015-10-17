#include <memory>
#include <vector>

class Widget: public std::enable_shared_from_this<Widget> {
public:
  template<typename... Ts>
  static std::shared_ptr<Widget> create(Ts&&... params);

  void process();
};

std::vector<std::shared_ptr<Widget>> processWidgets;

void Widget::process() {
  processWidgets.emplace_back(shared_from_this());
}

void makeLogEntry(Widget* w) {};

int main()
{
  auto loggingDel = [](Widget *pw)
                    {
                      makeLogEntry(pw);
                      delete pw;
                    };

  {
    std::unique_ptr<Widget, decltype(loggingDel)>
      upw(new Widget, loggingDel);

    std::shared_ptr<Widget>
      spw(new Widget, loggingDel);
  }

  {
    auto customDeleter1 = [](Widget *pw) {};
    auto customDeleter2 = [](Widget *pw) {};

    std::shared_ptr<Widget> pw1(new Widget, customDeleter1);
    std::shared_ptr<Widget> pw2(new Widget, customDeleter2);

    std::vector<std::shared_ptr<Widget>> vpw{ pw1, pw2 };
  }

  {
    auto pw = new Widget;

    std::shared_ptr<Widget> spw1(pw, loggingDel);
    // std::shared_ptr<Widget> spw2(pw, loggingDel); segmentation fault
  }

  {
    std::shared_ptr<Widget> spw1(new Widget, loggingDel);
    std::shared_ptr<Widget> spw2(spw1);
  }

  return 0;
}
