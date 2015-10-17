#include <memory>
#include <string>
#include <vector>
#include <cassert>

class Widget {
public:
  std::string doWork() & { return "doWork()&"; }   // *this is an lvalue
  std::string doWork() && { return "doWork()&&"; } // *this is an rvalue
};

Widget makeWidget()
{
  return Widget();
}

class Widget2 {
public:
  using DataType = std::vector<double>;

  DataType& data() { return values; }

private:
  DataType values;
};

Widget2 makeWidget2()
{
  return Widget2();
}

class Widget3 {
public:
  using DataType = std::vector<double>;

  DataType& data() &
  { return values; }

  DataType&& data() &&
  { return std::move(values); }

private:
  DataType values;
};

Widget3 makeWidget3()
{
  return Widget3();
}

int main()
{
  {
    class Base {
    public:
      virtual void doWork() {}
    };

    class Derived: public Base {
    public:
      virtual void doWork() {}
    };

    std::unique_ptr<Base> upb =
      std::make_unique<Derived>();

    upb->doWork();
  }

  {
    Widget w;

    assert(w.doWork() == "doWork()&");
    assert(makeWidget().doWork() == "doWork()&&");
  }

  {
    class Base {
    public:
      virtual void mf1() const;
      virtual void mf2(int x);
      virtual void mf3() &;
      void mf4() const;
    };

    class Derived: public Base {
    public:
      virtual void mf1();               // missing const
      virtual void mf2(unsigned int x); // argument type mismatch
      virtual void mf3() &&;            // & <-> &&
      void mf4() const;                 // missing virtual
    };
  }

  {
    class Base {
    public:
      virtual void mf1() const;
      virtual void mf2(int x);
      virtual void mf3() &;
      void mf4() const;
    };

    class Derived: public Base {
    public:
      // virtual void mf1() override;               missing const
      // virtual void mf2(unsigned int x) override; argument type int <-> unsigned int
      // virtual void mf3() && override;            & <-> &&
      // virtual void mf4() const override;         missing virtual
    };
  }

  {
    class Base {
    public:
      virtual void mf1() const;
      virtual void mf2(int x);
      virtual void mf3() &;
      virtual void mf4() const;
    };

    class Derived: public Base {
    public:
      virtual void mf1() const override;
      virtual void mf2(int x) override;
      virtual void mf3() & override;
      void mf4() const override;
    };
  }

  {
    class Warning {
    public:
      void override(); // function override name is valid
    };
  }

  {
    void doSomething(Widget& w);  // accepts only lvalue Widgets
    void doSomething(Widget&& w); // accepts only rvalue Widgets
  }

  {
    Widget2 w;
    auto vals1 = w.data();             // copy w.values into vals1
    auto vals2 = makeWidget2().data(); // copy values inside the Widget into vals2
  }

  {
    Widget3 w;
    auto vals1 = w.data();             // vals1 is copy-constructed
    auto vals2 = makeWidget3().data(); // vals2 is move-constructed
  }

  return 0;
}
