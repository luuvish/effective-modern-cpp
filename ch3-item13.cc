#include <algorithm>
#include <vector>

template<typename C, typename V>
void findAndInsert(C& container,
                   const V& targetVal,
                   const V& insertVal) {
  using std::cbegin;
  using std::cend;

  auto it = std::find(cbegin(container),
                      cend(container),
                      targetVal);

  container.insert(it, insertVal);
}

template<class C>
auto cbegin(const C& container) -> decltype(std::begin(container))
{
  return std::begin(container);
}

int main()
{
  {
    std::vector<int> values;

    std::vector<int>::iterator it =
      std::find(values.begin(), values.end(), 1983);
    values.insert(it, 1998);
  }

  {
    typedef std::vector<int>::iterator IterT;
    typedef std::vector<int>::const_iterator ConstIterT;

    std::vector<int> values;

    ConstIterT ci =
      std::find(static_cast<ConstIterT>(values.begin()),
                static_cast<ConstIterT>(values.end()),
                1983);
    // values.insert(static_cast<IterT>(ci), 1998); error!
  }

  {
    std::vector<int> values;

    auto it =
      std::find(values.cbegin(), values.cend(), 1983);
    values.insert(it, 1998);
  }

  {
    std::vector<int> values;

    findAndInsert(values, 1983, 1998);
  }

  return 0;
}
