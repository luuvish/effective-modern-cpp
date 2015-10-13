#include <cassert>
#include <unordered_map>
#include <vector>
#include <iostream>

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

template<typename T>
decltype(auto) pretty_name()
{
  return type_id_with_cvr<T>().pretty_name();
}

struct Widget {
  int x, y;
};

bool operator < (const Widget& w1, const Widget& w2)
{
  return (w1.x < w2.x) && (w1.y < w2.y);
}

template<typename It>
void dwim(It b, It e)
{
  for (; b != e; ++b) {
    typename std::iterator_traits<It>::value_type
      currValue = *b;
    auto currValue2 = *b;
  }
}

int main()
{
  {
    auto derefUPLess =
      [](const std::unique_ptr<Widget>& p1,
         const std::unique_ptr<Widget>& p2)
      { return *p1 < *p2; };

    auto derefLess =
      [](const auto& p1, const auto& p2)
      { return *p1 < *p2; };
  }

  {
    std::function<bool(const std::unique_ptr<Widget>&,
                       const std::unique_ptr<Widget>&)> func;

    std::function<bool(const std::unique_ptr<Widget>&,
                       const std::unique_ptr<Widget>&)>
      derefUPLess = [](const std::unique_ptr<Widget>& p1,
                       const std::unique_ptr<Widget>& p2)
                      { return *p1 < *p2; };
  }

  {
    std::vector<int> v;
    auto sz = v.size(); // sz's type is std::vector<int>::size_type
    assert(pretty_name<decltype(sz)>() == pretty_name<std::vector<int>::size_type>());
  }

  {
    std::unordered_map<std::string, int> m;
    for (const std::pair<std::string, int>& p : m) {}
    for (const auto& p : m) {}
  }

  return 0;
}
