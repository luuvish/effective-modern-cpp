#include <string>
#include <vector>
#include <memory>

class Widget11 {
public:
  void addName(const std::string& newName)
  { names.push_back(newName); }

  void addName(std::string&& newName)
  { names.push_back(std::move(newName)); }

private:
  std::vector<std::string> names;
};

class Widget12 {
public:
  template<typename T>
  void addName(T&& newName)
  { names.push_back(std::forward<T>(newName)); }

private:
  std::vector<std::string> names;
};

class Widget13 {
public:
  void addName(std::string newName)
  { names.push_back(std::move(newName)); }

private:
  std::vector<std::string> names;
};

class Widget21 {
public:
  void setPtr(std::unique_ptr<std::string>&& ptr)
  { p = std::move(ptr); }

private:
  std::unique_ptr<std::string> p;
};

class Widget22 {
public:
  void setPtr(std::unique_ptr<std::string> ptr)
  { p = std::move(ptr); }

private:
  std::unique_ptr<std::string> p;
};

class Widget31 {
public:
  void addName(std::string newName) {
    if ((newName.length() >= minLen) &&
        (newName.length() <= maxLen)) {
      names.push_back(std::move(newName));
    }
  }

private:
  std::vector<std::string> names;
  const int minLen = 0, maxLen = 10;
};

class Password1 {
public:
  explicit Password1(std::string pwd)
  : text(std::move(pwd)) {}

  void changeTo(std::string newPwd)
  { text = std::move(newPwd); }

private:
  std::string text;
};

class Password2 {
public:
  explicit Password2(std::string pwd)
  : text(std::move(pwd)) {}

  void changeTo(const std::string& newPwd)
  { text = newPwd; }

private:
  std::string text;
};

int main()
{
  {
    Widget11 w;
    std::string name("Bart");
    w.addName(name);           // call addName with lvalue
    w.addName(name + "Jenne"); // call addName with rvalue
  }

  {
    Widget21 w;
    w.setPtr(std::make_unique<std::string>("Modern C++"));
  }

  {
    std::string initPwd("Supercalifragilisticexpialidocious");
    Password1 p(initPwd);

    std::string newPassword = "Beware the Jabberwock";
    p.changeTo(newPassword);
  }

  return 0;
}
