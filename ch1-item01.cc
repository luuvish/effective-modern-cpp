#include <array>
#include <tuple>
#include <string>
#include <cassert>

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

/*
  template<typename T>
  void f(ParamType param);

  f(expr); // deduce T and ParamType from expr
 */

template<typename T>
decltype(auto) case1_ref(T& param)
{
  return std::tuple<std::string, std::string>(
    type_id_with_cvr<T>().pretty_name(),
    type_id_with_cvr<decltype(param)>().pretty_name()
  );
}

template<typename T>
decltype(auto) case1_cref(const T& param)
{
  return std::tuple<std::string, std::string>(
    type_id_with_cvr<T>().pretty_name(),
    type_id_with_cvr<decltype(param)>().pretty_name()
  );
}

template<typename T>
decltype(auto) case1_ptr(T* param)
{
  return std::tuple<std::string, std::string>(
    type_id_with_cvr<T>().pretty_name(),
    type_id_with_cvr<decltype(param)>().pretty_name()
  );
}

template<typename T>
decltype(auto) case2_uref(T&& param)
{
  return std::tuple<std::string, std::string>(
    type_id_with_cvr<T>().pretty_name(),
    type_id_with_cvr<decltype(param)>().pretty_name()
  );
}

template<typename T>
decltype(auto) case3_val(T param)
{
  return std::tuple<std::string, std::string>(
    type_id_with_cvr<T>().pretty_name(),
    type_id_with_cvr<decltype(param)>().pretty_name()
  );
}

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
  return N;
}

void someFunc(int, double)
{
}

int main()
{
  std::string T, param;

  // Case 1: ParamType is a Reference or Pointer, but not a Universal
  {
    // template<typename T> void case1_ref(T& param);
    int x = 27;
    const int cx = x;
    const int& rx = x;

    std::tie (T, param) = case1_ref(x);  assert(T == "int"       && param == "int&");
    std::tie (T, param) = case1_ref(cx); assert(T == "int const" && param == "int const&");
    std::tie (T, param) = case1_ref(rx); assert(T == "int const" && param == "int const&");
  }
  {
    // template<typename T> void case1_cref(const T& param);
    int x = 27;
    const int cx = x;
    const int& rx = x;

    std::tie (T, param) = case1_cref(x);  assert(T == "int" && param == "int const&");
    std::tie (T, param) = case1_cref(cx); assert(T == "int" && param == "int const&");
    std::tie (T, param) = case1_cref(rx); assert(T == "int" && param == "int const&");
  }
  {
    // template<typename T> void case1_ptr(T* param);
    int x = 27;
    const int *px = &x;

    std::tie (T, param) = case1_ptr(&x); assert(T == "int"       && param == "int*");
    std::tie (T, param) = case1_ptr(px); assert(T == "int const" && param == "int const*");
  }

  // Case 2: ParamType is a Universal Reference
  {
    // template<typename T> void case2_uref(T&& param);
    int x = 27;
    const int cx = x;
    const int& rx = x;

    std::tie (T, param) = case2_uref(x);  assert(T == "int&"       && param == "int&");
    std::tie (T, param) = case2_uref(cx); assert(T == "int const&" && param == "int const&");
    std::tie (T, param) = case2_uref(rx); assert(T == "int const&" && param == "int const&");
    std::tie (T, param) = case2_uref(27); assert(T == "int"        && param == "int&&");
  }

  // Case 3: ParamType is Neither a Pointer nor a Reference
  {
    // template<typename T> void case3_val(T param);
    int x = 27;
    const int cx = x;
    const int& rx = x;

    std::tie (T, param) = case3_val(x);  assert(T == "int" && param == "int");
    std::tie (T, param) = case3_val(cx); assert(T == "int" && param == "int");
    std::tie (T, param) = case3_val(rx); assert(T == "int" && param == "int");
  }
  {
    const char* const ptr = "Fun with pointers";
    std::tie (T, param) = case3_val(ptr); assert(T == "char const*" && param == "char const*");
  }

  // Array
  {
    const char name[] = "J. P. Briggs";

    // template<typename T> void case3_val(T param);
    std::tie (T, param) = case3_val(name);
    assert(T == "char const*" && param == "char const*");

    // template<typename T> void case1_ref(T& param);
    std::tie (T, param) = case1_ref(name);
    assert(T == "char const [13]" && param == "char const (&) [13]");
  }
  {
    // template<typename T, std::size_t N> constexpr std::size_t arraySize(T (&)[N]) noexcept;
    int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };
    int mappedVals[arraySize(keyVals)];
    std::array<int, arraySize(keyVals)> mappedVals2;
    assert(arraySize(keyVals) == 7);
  }

  // Function
  {
    // void someFunc(int, double);

    // template<typename T> void case3_val(T param);
    std::tie (T, param) = case3_val(someFunc);
    assert(T == "void (*)(int, double)" && param == "void (*)(int, double)");

    // template<typename T> void case1_ref(T& param);
    std::tie (T, param) = case1_ref(someFunc);
    assert(T == "void (int, double)" && param == "void (&)(int, double)");
  }

  return 0;
}
