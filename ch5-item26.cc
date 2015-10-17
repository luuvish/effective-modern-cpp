#include <string>
#include <set>
#include <chrono>

void log(std::chrono::system_clock::time_point, std::string) {}

std::multiset<std::string> names;

void logAndAdd(const std::string& name)
{
  auto now = std::chrono::system_clock::now();
  log(now, "logAndAdd");
  names.emplace(name);
}

template<typename T>
void logAndAdd2(T&& name)
{
  auto now = std::chrono::system_clock::now();
  log(now, "logAndAdd");
  names.emplace(std::forward<T>(name));
}

std::string nameFromIdx(int idx) { return ""; }

void logAndAdd2(int idx)
{
  auto now = std::chrono::system_clock::now();
  log(now, "logAndAdd");
  names.emplace(nameFromIdx(idx));
}

class Person {
public:
  template<typename T>
  explicit Person(T&& n)
  : name(std::forward<T>(n)) {}

  explicit Person(int idx)
  : name(nameFromIdx(idx)) {}

private:
  std::string name;
};

class SpecialPerson : public Person {
public:
  // SpecialPerson(const SpecialPerson& rhs) // perfect forward constructor
  // : Person(rhs)
  // {}

  // SpecialPerson(SpecialPerson&& rhs) // perfect forward constructor
  // : Person(std::move(rhs))
  // {}
};

int main()
{
  {
    std::string petName("Darla");

    logAndAdd(petName);                   // pass lvalue std::string
    logAndAdd(std::string("Persephone")); // pass rvalue std::string
    logAndAdd("Patty Dog");               // pass string literal
  }

  {
    std::string petName("Darla");

    logAndAdd2(petName);                   // copy lvalue into multiset
    logAndAdd2(std::string("Persephone")); // move rvalue instead of copying it
    logAndAdd2("Patty Dog");               // copy std::string in multiset instead
                                           // of copying a temporary std::string
  }

  {
    std::string petName("Darla");

    logAndAdd2(petName);
    logAndAdd2(std::string("Persephone"));
    logAndAdd2("Patty Dog");

    logAndAdd2(22); // calls int overload

    short nameIdx;
    // logAndAdd2(nameIdx); error!
  }

  {
    Person p("Nancy");
    // auto cloneOfP(p); error!

    const Person cp("Nancy");
    auto cloneOfP(cp);
  }

  return 0;
}
