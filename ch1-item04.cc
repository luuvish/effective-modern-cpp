#include <vector>
#include <tuple>
#include <string>
#include <cassert>

template<typename T>
decltype(auto) simple_typeid(const T& param)
{
    return std::tuple<std::string, std::string>(
        typeid(T).name(),
        typeid(param).name()
    );
}

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

template<typename T>
decltype(auto) pretty_typeid(const T& param)
{
    return std::tuple<std::string, std::string>(
        type_id_with_cvr<T>().pretty_name(),
        type_id_with_cvr<decltype(param)>().pretty_name()
    );
}

class Widget {
    int x, y;
};

std::vector<Widget> createVec()
{
    return std::vector<Widget>(10);
}

int main()
{
    {
        const int theAnswer = 42;
        auto x = theAnswer;
        auto y = &theAnswer;

        assert(std::string(typeid(x).name()) == "i");
        assert(std::string(typeid(y).name()) == "PKi");
    }

    {
        const auto vw = createVec();

        if (!vw.empty()) {
            std::string T, param;
            std::tie (T, param) = simple_typeid(&vw[0]);
            assert(T == "PK6Widget");
            assert(param == "PK6Widget");
        }
    }

    {
        const auto vw = createVec();

        if (!vw.empty()) {
            std::string T, param;
            std::tie (T, param) = pretty_typeid(&vw[0]);
            assert(T == "Widget const*");
            assert(param == "Widget const* const&");
        }
    }

    return 0;
}
