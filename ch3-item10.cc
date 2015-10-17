#include <cstdint>
#include <tuple>
#include <vector>
#include <string>

std::vector<std::size_t>
primeFactors(std::size_t size) {
  return std::vector<std::size_t>{ 1, 2, 3, 4 };
}

template<typename E>
constexpr typename std::underlying_type<E>::type
  toUType1(E enumerator) noexcept
{
  return
    static_cast<typename
                std::underlying_type<E>::type>(enumerator);
}

template<typename E>
constexpr std::underlying_type_t<E>
  toUType2(E enumerator) noexcept
{
  return static_cast<std::underlying_type_t<E>>(enumerator);
}

template<typename E>
constexpr auto
  toUType3(E enumerator) noexcept
{
  return static_cast<std::underlying_type_t<E>>(enumerator);
}

int main()
{
  {
    enum Color { black, white, red };
    // auto white = false; error!
  }

  {
    enum class Color { black, white, red };
    auto white = false;
    // Color c = white; error!
    Color c = Color::white;
    auto c1 = Color::white;
  }

  {
    enum Color { black, white, red };

    Color c = red;
    if (c < 14.5) {
      auto factors = primeFactors(c);
    }
  }

  {
    enum class Color { black, white, red };

    Color c = Color::red;
    if (static_cast<double>(c) < 14.5) {
      auto factors = primeFactors(static_cast<std::size_t>(c));
    }
  }

  {
    enum class Status: std::uint32_t;
    void contineProcessing(Status s);

    enum Color: std::uint8_t;
    enum class Statue: std::uint32_t { good = 0,
                                       failed = 1,
                                       incomplete = 100,
                                       corrupt = 200,
                                       audited = 500,
                                       indeterminate = 0xFFFFFFFF
                                     };
  }

  using UserInfo =
    std::tuple<std::string,
               std::string,
               std::size_t>;

  {
    UserInfo uInfo;
    auto val = std::get<1>(uInfo);
  }
  {
    enum UserInfoFields { uiName, uiEmail, uiReputation };
    UserInfo uInfo;
    auto val = std::get<uiEmail>(uInfo);
  }
  {
    enum class UserInfoFields { uiName, uiEmail, uiReputation };
    UserInfo uInfo;
    auto val =
      std::get<static_cast<std::size_t>(UserInfoFields::uiEmail)>
        (uInfo);
  }

  {
    enum class UserInfoFields { uiName, uiEmail, uiReputation };
    UserInfo uInfo;
    auto val1 = std::get<toUType1(UserInfoFields::uiEmail)>(uInfo);
    auto val2 = std::get<toUType2(UserInfoFields::uiEmail)>(uInfo);
    auto val3 = std::get<toUType3(UserInfoFields::uiEmail)>(uInfo);
  }

  return 0;
}
