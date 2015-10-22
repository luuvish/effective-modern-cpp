#include <utility>

template<typename T>
T normalize(T t) { return t; }

class SomeCompilerGeneratedClassName {
public:
  template<typename T>
  auto operator()(T x) const
  { return normalize(x); }
};

int main()
{
  auto f1 = [](auto x){ return normalize(x); };

  auto f2 = [](auto&& x)
            {
              return
                normalize(std::forward<decltype(x)>(x));
            };

  auto f3 = [](auto&&... xs)
            {
              return
                normalize(std::forward<decltype(xs)>(xs)...);
            };

  return 0;  
}
