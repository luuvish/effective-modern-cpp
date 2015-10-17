#include <cassert>
#include <memory>
#include <unordered_map>
#include <list>

typedef
  std::unique_ptr<std::unordered_map<std::string, std::string>>
  UPtrMapSS;

using UPtrMapSS =
  std::unique_ptr<std::unordered_map<std::string, std::string>>;

typedef void (*FP)(int, const std::string&);
using FP = void (*)(int, const std::string&);

template<typename T>
class MyAlloc : public std::allocator<T> {
};

template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

template<typename T>
struct MyAllocList2 {
  typedef std::list<T, MyAlloc<T>> type;
};

template<typename T>
class Widget {
private:
  MyAllocList<T> list;
};

template<typename T>
class Widget2 {
private:
  typename MyAllocList2<T>::type list;
};

class Wine {};

template<>
class MyAllocList2<Wine> {
private:
  enum class WineType { White, Red, Rose };
  WineType type;
};

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

template<typename T>
decltype(auto) pretty_name()
{
  return type_id_with_cvr<T>().pretty_name();
}

template<class T>
using remove_const_t = typename std::remove_const<T>::type;

template<class T>
using remove_reference_t = typename std::remove_reference<T>::type;

template<class T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

int main()
{
  {
    class Widget {
      int x, y;
    };

    MyAllocList<Widget> lw;
    MyAllocList2<Widget>::type lw2;
  }

  {
    assert(pretty_name<std::remove_const<const int>::type>() == "int");
    assert(pretty_name<std::remove_reference<int&>::type>() == "int");
    assert(pretty_name<std::add_lvalue_reference<int>::type>() == "int&");
  }

  {
    assert(pretty_name<std::remove_const_t<const int>>() == "int");
    assert(pretty_name<std::remove_reference_t<int&>>() == "int");
    assert(pretty_name<std::add_lvalue_reference_t<int>>() == "int&");
  }

  return 0;
}
