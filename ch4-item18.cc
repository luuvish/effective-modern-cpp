#include <memory>

class Investment {
public:
  virtual ~Investment() {};
};

class Stock : public Investment {};
class Bond : public Investment {};
class RealEstate : public Investment {};

template<typename... Ts>
std::unique_ptr<Investment>
makeInvestment(Ts&&... params)
{
  return std::unique_ptr<Investment>
    (new Investment(std::forward<Ts>(params)...));
}

void makeLogEntry(Investment* pInvestment) {};

auto delInvmt = [](Investment* pInvestment) {
                  makeLogEntry(pInvestment);
                  delete pInvestment;
                };

// c++11
template<typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt)>
makeInvestment2(Ts&&... params)
{
  std::unique_ptr<Investment, decltype(delInvmt)>
    pInv(nullptr, delInvmt);

  if (1) {
    pInv.reset(new Stock(std::forward<Ts>(params)...));
  } else if (2) {
    pInv.reset(new Bond(std::forward<Ts>(params)...));
  } else if (3) {
    pInv.reset(new RealEstate(std::forward<Ts>(params)...));
  }

  return pInv;
}

// c++14
template<typename... Ts>
auto makeInvestment3(Ts&&... params)
{
  auto delInvmt = [](Investment* pInvestment) {
                    makeLogEntry(pInvestment);
                    delete pInvestment;
                  };

  std::unique_ptr<Investment, decltype(delInvmt)>
    pInv(nullptr, delInvmt);

  if (1) {
    pInv.reset(new Stock(std::forward<Ts>(params)...));
  } else if (2) {
    pInv.reset(new Bond(std::forward<Ts>(params)...));
  } else if (3) {
    pInv.reset(new RealEstate(std::forward<Ts>(params)...));
  }

  return pInv;
}

auto delInvmt1 = [](Investment* pInvestment)
                 {
                   makeLogEntry(pInvestment);
                   delete pInvestment;
                 };

template<typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt1)>
makeInvestment4(Ts&&... args);

void delInvmt2(Investment* pInvestment)
{
  makeLogEntry(pInvestment);
  delete pInvestment;
}

template<typename... Ts>
std::unique_ptr<Investment,
                void (*)(Investment*)>
makeInvestment5(Ts&&... params);

int main()
{
  {
    auto pInvestment = makeInvestment();
    auto pInvestment2 = makeInvestment2();
    auto pInvestment3 = makeInvestment3();
  }

  {
    std::shared_ptr<Investment> sp =
      makeInvestment();
  }

  return 0;
}
