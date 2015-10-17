#include <string>
#include <vector>
#include <memory>

int main()
{
  {
    // #include "gadget.h"
    class Gadget {};

    class Widget {
    public:
      Widget();

    private:
      std::string name;
      std::vector<double> data;
      Gadget g1, g2, g3;
    };
  }

  {
    class Widget {
    public:
      Widget();
      ~Widget();

    private:
      struct Impl;
      Impl *pImpl;
    };

    // #include "widget.h"
    // #include "gadget.h"
    class Gadget {};

    struct Widget::Impl {
      std::string name;
      std::vector<double> data;
      Gadget g1, g2, g3;
    };

    // Widget::Widget(): pImpl(new Impl) {}
    // Widget::~Widget() { delete pImpl; }
  }

  {
    class Widget {
    public:
      Widget();

    private:
      struct Impl;
      std::unique_ptr<Impl> pImpl;
    };

    // #include "widget.h"
    // #include "gadget.h"
    class Gadget {};

    struct Widget::Impl {
      std::string name;
      std::vector<double> data;
      Gadget g1, g2, g3;
    };

    // Widget::Widget(): pImpl(std::make_unique<Impl>()) {}
  }

  {
    class Widget {
    public:
      Widget();
      ~Widget();

    private:
      struct Impl;
      std::unique_ptr<Impl> pImpl;
    };

    // #include "widget.h"
    // #include "gadget.h"
    class Gadget {};

    struct Widget::Impl {
      std::string name;
      std::vector<double> data;
      Gadget g1, g2, g3;
    };

    // Widget::Widget(): pImpl(std::make_unique<Impl>()) {}
    // Widget::~Widget() {}
  }

  {
    class Widget {
    public:
      Widget();
      ~Widget();

      Widget(Widget&& rhs);
      Widget& operator=(Widget&& rhs);

    private:
      struct Impl;
      std::unique_ptr<Impl> pImpl;
    };

    // #include "widget.h"
    // #include "gadget.h"
    class Gadget {};

    struct Widget::Impl {
      std::string name;
      std::vector<double> data;
      Gadget g1, g2, g3;
    };

    // Widget::Widget(): pImpl(std::make_unique<Impl>()) {}
    // Widget::~Widget() = default;

    // Widget::Widget(Widget&& rhs) = default;
    // Widget& Widget::operator=(Widget&& rhs) = default;
  }

  {
    class Widget {
    public:
      Widget();
      ~Widget();

      Widget(Widget&& rhs);
      Widget& operator=(Widget&& rhs);

      Widget(const Widget& rhs);
      Widget& operator=(const Widget& rhs);

    private:
      struct Impl;
      std::unique_ptr<Impl> pImpl;
    };

    // #include "widget.h"
    // #include "gadget.h"
    class Gadget {};

    struct Widget::Impl {
      std::string name;
      std::vector<double> data;
      Gadget g1, g2, g3;
    };

    // Widget::Widget(): pImpl(std::make_unique<Impl>()) {}
    // Widget::~Widget() = default;

    // Widget::Widget(Widget&& rhs) = default;
    // Widget& Widget::operator=(Widget&& rhs) = default;

    // Widget::Widget(const Widget& rhs)
    // : pImpl(nullptr)
    // { if (rhs.pImpl) pImpl = std::make_unique<Impl>(*rhs.pImpl); }

    // Widget& Widget::operator=(const Widget& rhs)
    // {
    //   if (!rhs.pImpl) pImpl.reset();
    //   else if (!pImpl) pImpl = std::make_unique<Impl>(*rhs.pImpl);
    //   else *pImpl = *rhs.pImpl;
    //   return *this;
    // }
  }

  {
    class Widget {
    public:
      Widget() {}

    private:
      struct Impl;
      std::shared_ptr<Impl> pImpl;
    };

    Widget w1;
    auto w2(std::move(w1));
    w1 = std::move(w2);
  }

  return 0;
}
