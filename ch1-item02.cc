#include <cassert>

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

template<typename T>
decltype(auto) pretty_name()
{
    return type_id_with_cvr<T>().pretty_name();
}

void someFunc(int, double)
{
}

template<typename T>
decltype(auto) f1(T param)
{
    return pretty_name<decltype(param)>();
}

template<typename T>
decltype(auto) f2(std::initializer_list<T> initList)
{
    return pretty_name<decltype(initList)>();
}

int main()
{
    {
        auto x = 27;
        const auto cx = x;
        const auto& rx = x;

        assert(pretty_name<decltype(x)>() == "int");
        assert(pretty_name<decltype(cx)>() == "int const");
        assert(pretty_name<decltype(rx)>() == "int const&");

        auto&& uref1 = x;
        auto&& uref2 = cx;
        auto&& uref3 = 27;

        assert(pretty_name<decltype(uref1)>() == "int&");
        assert(pretty_name<decltype(uref2)>() == "int const&");
        assert(pretty_name<decltype(uref3)>() == "int&&");

        const char name[] = "R. N. Briggs";
        auto arr1 = name;
        auto& arr2 = name;

        assert(pretty_name<decltype(name)>() == "char const [13]");
        assert(pretty_name<decltype(arr1)>() == "char const*");
        assert(pretty_name<decltype(arr2)>() == "char const (&) [13]");

        // void someFunc(int, double); type is void(int, double)
        auto func1 = someFunc;
        auto& func2 = someFunc;

        assert(pretty_name<decltype(someFunc)>() == "void (int, double)");
        assert(pretty_name<decltype(func1)>() == "void (*)(int, double)");
        assert(pretty_name<decltype(func2)>() == "void (&)(int, double)");
    }

    {
        auto x1 = 27;
        auto x2(27);
        auto x3 = { 27 };
        // auto x4{ 27 }; -Wfuture-compat

        assert(pretty_name<decltype(x1)>() == "int");
        assert(pretty_name<decltype(x2)>() == "int");
        assert(pretty_name<decltype(x3)>() == "std::initializer_list<int>");

        auto x = { 11, 23, 9 };
        assert(f1(x) == "std::initializer_list<int>");
        assert(f2({ 11, 23, 9 }) == "std::initializer_list<int>");
    }

    return 0;
}
