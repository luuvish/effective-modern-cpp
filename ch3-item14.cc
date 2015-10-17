#include <cstddef>
#include <string>

int f(int x) throw();  // c++98
int f(int x) noexcept; // c++11

template<class T, size_t N>
void swap(T (&a)[N],
          T (&b)[N]) noexcept(noexcept(swap(*a, *b)));

template<class T1, class T2>
struct pair {
  void swap(pair& p) noexcept(noexcept(swap(first, p.first)) &&
                              noexcept(swap(second, p.second)));
  T1 first;
  T2 second;
};

void f(const std::string& s) noexcept; // s.length() <= 32

void setup() {}
void cleanup() {}

void doWork() noexcept
{
  setup();

  cleanup();
}

int main()
{
  return 0;
}
