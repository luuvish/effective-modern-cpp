
bool isLucky(int number) { return number == 7; }
bool isLucky(char) = delete;
bool isLucky(bool) = delete;
bool isLucky(double) = delete;

template<typename T>
void processPointer(T* ptr);

template<>
void processPointer<void>(void*) = delete;
template<>
void processPointer<char>(char*) = delete;
template<>
void processPointer<const void>(const void*) = delete;
template<>
void processPointer<const char>(const char*) = delete;

class Widget {
public:
  template<typename T>
  void processPointer(T* ptr) {}
};

template<>
void Widget::processPointer<void>(void*) = delete;

int main()
{
  // isLucky('a'); error!
  // isLucky(true); error!
  // isLucky(3.5f); error!

  return 0;
}
