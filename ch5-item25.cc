#include <string>
#include <memory>
#include <chrono>

class Widget1 {
public:
  Widget1(Widget1&& rhs) // rvalue reference
  : name(std::move(rhs.name)),
    p(std::move(rhs.p))
  {}

private:
  std::string name;
  std::shared_ptr<int> p;
};

class Widget2 {
public:
  template<typename T>
  void setName(T&& newName) // universal reference
  { name = std::forward<T>(newName); }

private:
  std::string name;
};

class Widget3 {
public:
  template<typename T>
  void setName(T&& newName) // universal reference
  { name = std::move(newName); } // bad!

private:
  std::string name;
};

class Widget4 {
public:
  void setName(const std::string& newName)
  { name = newName; }
  void setName(std::string&& newName)
  { name = std::move(newName); }

private:
  std::string name;
};

std::string getWidgetName() { return ""; }

template<class T, class... Args>
std::shared_ptr<T> make_shared(Args&&... args);

template<class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args);

template<typename T>
void setSignText(T&& text) {
  // sign.setText(text);

  auto now = std::chrono::system_clock::now();

  // signHistory.add(now, std::forward<T>(text));
}

class Matrix {
public:
  Matrix& operator+=(const Matrix& rhs) { return *this; }
};

Matrix
operator+(Matrix&& lhs, const Matrix& rhs) {
  lhs += rhs;
  return std::move(lhs);
}

class Fraction {
public:
  void reduce() {};
};

template<typename T>
Fraction
reduceAndCopy(T&& frac) {
  frac.reduce();
  return std::forward<T>(frac);
}

Widget2 makeWidget()
{
  Widget2 w;

  // return std::move(w); error!
  return w;
}

int main()
{
  {
    Widget2 w;
    auto n = getWidgetName(); // n is local variable
    w.setName(n);             // n's value now unknown
  }

  {
    Widget4 w;
    w.setName("Adela Novak");
  }

  return 0;
}
