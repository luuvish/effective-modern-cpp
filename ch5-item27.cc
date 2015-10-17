#include <string>
#include <set>
#include <chrono>

void log(std::chrono::system_clock::time_point, std::string) {}

std::string nameFromIdx(int idx) { return ""; }

std::multiset<std::string> names;

template<typename T>
void logAndAdd(T&& name)
{
  auto now = std::chrono::system_clock::now();
  log(now, "logAndAdd");
  names.emplace(std::forward<T>(name));
}

template<typename T>
void logAndAddImpl(T&& name, std::false_type);
void logAndAddImpl(int idx, std::true_type);

template<typename T>
void logAndAdd2(T&& name)
{
  logAndAddImpl(
    std::forward<T>(name),
    std::is_integral<typename std::remove_reference<T>::type>()
  );
}

template<typename T>
void logAndAddImpl(T&& name, std::false_type)
{
  auto now = std::chrono::system_clock::now();
  log(now, "logAndAdd");
  names.emplace(std::forward<T>(name));
}

void logAndAddImpl(int idx, std::true_type)
{
  logAndAdd2(nameFromIdx(idx));
}

class Person {
public:
  template<
    typename T,
    typename = typename std::enable_if<
                 !std::is_same<Person,
                               typename std::decay<T>::type
                              >::value
               >::type
  >
  explicit Person(T&& n) {}
};

// c++11
class Person2 {
public:
  template<
    typename T,
    typename = typename std::enable_if<
                 !std::is_base_of<Person2,
                                  typename std::decay<T>::type
                                 >::value
               >::type
  >
  explicit Person2(T&& n) {}
};

// c++14
class Person3 {
public:
  template<
    typename T,
    typename = std::enable_if_t<
                 !std::is_base_of<Person3,
                                  std::decay_t<T>
                                 >::value
               >
  >
  explicit Person3(T&& n) {}
};

class Person4 {
public:
  template<
    typename T,
    typename = std::enable_if_t<
      !std::is_base_of<Person4, std::decay_t<T>>::value
      &&
      !std::is_integral<std::remove_reference_t<T>>::value
    >
  >
  explicit Person4(T&& n)
  : name(std::forward<T>(n)) {}

  explicit Person4(int idx)
  : name(nameFromIdx(idx)) {}

private:
  std::string name;
};

class Person5 {
public:
  template<
    typename T,
    typename = std::enable_if_t<
      !std::is_base_of<Person5, std::decay_t<T>>::value
      &&
      !std::is_integral<std::remove_reference_t<T>>::value
    >
  >
  explicit Person5(T&& n)
  : name(std::forward<T>(n)) {
    static_assert(
      std::is_constructible<std::string, T>::value,
      "Parameter n can't be used to construct a std::string"
    );
  }

  explicit Person5(int idx)
  : name(nameFromIdx(idx)) {}

private:
  std::string name;
};

int main()
{
  {
    class Person {
    public:
      explicit Person(std::string n)
      : name(std::move(n)) {}

      explicit Person(int idx)
      : name(nameFromIdx(idx)) {}

    private:
      std::string name;
    };
  }

  {
    logAndAdd("logAndAdd");
    // logAndAdd(10); error!
  }

  {
    logAndAdd2("logAndAdd2");
    logAndAdd2(20);
  }

  {
    class SpecialPerson: public Person {
    public:
      SpecialPerson(const SpecialPerson& rhs)
      : Person(rhs) // call perfect forward constructor
      {}

      SpecialPerson(SpecialPerson&& rhs)
      : Person(std::move(rhs)) // call perfect forward constructor
      {}
    };
  }

  {
    // Person4 p(u"Konrad Zuse"); const char16_t
    // Person5 p(u"Konrad Zuse"); const char16_t
  }

  return 0;
}
