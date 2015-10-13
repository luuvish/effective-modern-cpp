#include <cassert>
#include <deque>

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

template<typename T>
decltype(auto) pretty_name()
{
  return type_id_with_cvr<T>().pretty_name();
}

class Widget {
  int x, y;
};

void authenticateUser()
{
}

// c++11
template<typename Container, typename Index>
auto
authAndAccess1(Container&& c, Index i)
-> decltype(std::forward<Container>(c)[i])
{
  authenticateUser();
  return std::forward<Container>(c)[i];
}

// c++14
template<typename Container, typename Index>
decltype(auto)
authAndAccess2(Container&& c, Index i)
{
  authenticateUser();
  return std::forward<Container>(c)[i];
}

std::deque<std::string> makeStringDeque()
{
  return std::deque<std::string>(10);
}

int main()
{
  Widget w;
  const Widget& cw = w;
  auto myWidget1 = cw;           // myWidget1's type is Widget
  decltype(auto) myWidget2 = cw; // myWidget2's type is const Widget&

  assert(pretty_name<decltype(w)>() == "Widget");
  assert(pretty_name<decltype(cw)>() == "Widget const&");
  assert(pretty_name<decltype(myWidget1)>() == "Widget");
  assert(pretty_name<decltype(myWidget2)>() == "Widget const&");

  auto s = authAndAccess2(makeStringDeque(), 5);

  int x = 0;

  assert(pretty_name<decltype(x)>() == "int");
  assert(pretty_name<decltype((x))>() == "int&");

  return 0;
}
