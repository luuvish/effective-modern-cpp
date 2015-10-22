#include <memory>
#include <vector>
#include <functional>

int main()
{
  class Widget {
  public:
    bool isValidated() const { return true; }
    bool isProcessed() const { return false; }
    bool isArchived() const { return true; }

  private:
  };

  { // c++14
    auto pw = std::make_unique<Widget>();

    auto func = [pw = std::move(pw)]
                { return pw->isValidated()
                         && pw->isArchived(); };

    auto func2 = [pw = std::make_unique<Widget>()]
                 { return pw->isValidated()
                          && pw->isArchived(); };

  }

  { // c++11
    class IsValAndArch {
    public:
      using DataType = std::unique_ptr<Widget>;

      explicit IsValAndArch(DataType&& ptr)
      : pw(std::move(ptr)) {}

      bool operator()() const
      { return pw->isValidated() && pw->isArchived(); }

    private:
      DataType pw;
    };

    auto func = IsValAndArch(std::make_unique<Widget>());
  }

  {
    std::vector<double> data;

    // c++14
    auto func1 = [data = std::move(data)]
                 {};

    // c++11
    auto func2 =
      std::bind(
        [](const std::vector<double>& data)
        {},
        std::move(data)
      );
    auto func3 =
      std::bind(
        [](std::vector<double>& data) mutable
        {},
        std::move(data)
      );
  }

  {
    // c++14
    auto func1 = [pw = std::make_unique<Widget>()]
                 { return pw->isValidated()
                          && pw->isArchived(); };

    // c++11
    auto func2 = std::bind(
                   [](const std::unique_ptr<Widget>& pw)
                   { return pw->isValidated()
                            && pw->isArchived(); },
                   std::make_unique<Widget>()
                 );
  }

  return 0;
}
