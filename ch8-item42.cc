#include <vector>
#include <list>
#include <string>
#include <memory>
#include <regex>

class Widget {};

void killWidget(Widget* pWidget) {}

int main()
{
  {
    std::vector<std::string> vs;

    vs.push_back("xyzzy");
    vs.push_back(std::string("xyzzy"));

    vs.emplace_back("xyzzy");
    vs.emplace_back(50, 'x');
  }

  {
    std::vector<std::string> vs;

    std::string queenOfDisco("Donna Summer");
    vs.push_back(queenOfDisco);
    vs.emplace_back(queenOfDisco);
  }

  {
    std::vector<std::string> vs;

    vs.emplace(vs.begin(), "xyzzy");    
  }

  {
    std::list<std::shared_ptr<Widget>> ptrs;

    ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
    ptrs.push_back({ new Widget, killWidget });

    ptrs.emplace_back(new Widget, killWidget);
  }

  {
    std::list<std::shared_ptr<Widget>> ptrs;

    std::shared_ptr<Widget> spw(new Widget, killWidget);
    ptrs.push_back(std::move(spw));
  }
  {
    std::list<std::shared_ptr<Widget>> ptrs;

    std::shared_ptr<Widget> spw(new Widget, killWidget);
    ptrs.emplace_back(std::move(spw));
  }

  {
    std::vector<std::regex> regexes;

    // regexes.emplace_back(nullptr); ?
    // std::regex r = nullptr;     error!
    // regexes.push_back(nullptr); error!

    std::regex upperCaseWord("[A-Z]+");

    // std::regex r(nullptr);   compile, but
    // std::regex r1 = nullptr; error!
    // std::regex r2(nullptr);  compile, but

    // regexes.emplace_back(nullptr) compile, but
    // regexes.push_back(nullptr)    error!
  }

  return 0;
}
