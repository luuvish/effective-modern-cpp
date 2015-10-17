#include <string>
#include <map>

void makeLogEntry(std::string s) {}

int main()
{
  {
    class Widget {
    public:
      Widget(Widget&& rhs);            // move constructor
      Widget& operator=(Widget&& rhs); // move assignment operator
    };
  }

  {
    class Widget {
    public:
      ~Widget(); // dtor

      Widget(const Widget&) = default;
      Widget& operator=(const Widget&) = default;
    };

    class Base {
    public:
      virtual ~Base() = default;

      Base(Base&&) = default;            // support moving
      Base& operator=(Base&&) = default;

      Base(const Base&) = default;            // support copying
      Base& operator=(const Base&) = default;
    };
  }

  {
    class StringTable {
    public:
      StringTable() {}
    private:
      std::map<int, std::string> values;
    };
  }

  {
    class StringTable {
    public:
      StringTable()
      { makeLogEntry("Creating StringTable object"); }
      ~StringTable()
      { makeLogEntry("Destroying StringTable object"); }

    private:
      std::map<int, std::string> values;
    };
  }

  return 0;
}
