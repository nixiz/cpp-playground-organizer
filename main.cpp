#include "cpp_organizer.hpp"
#include "constexpr_test.hpp"

using namespace CppOrganizer;

class DummyTestClass : public PlaygroundHelper<DummyTestClass>
{
public:
  DummyTestClass();
  ~DummyTestClass();

  void RunTest();

};

DummyTestClass::DummyTestClass() : PlaygroundHelper("Sample Test Class")
{
}

DummyTestClass::~DummyTestClass()
{
}

void DummyTestClass::RunTest()
{
  std::cout << "Dummy Test Runned" << std::endl;
}


class CommaOperatorTest : public PlaygroundHelper<CommaOperatorTest>
{
public:
  CommaOperatorTest() : PlaygroundHelper("CommaOperatorTest") {}
  ~CommaOperatorTest() = default;

  void RunTest() {
    int x = (5, 4, 3, 2, 1, 0);
    printf("x = %d", x);
  }

};

std::vector<IPlaygroundObj> CppOrganizer::_default_objects = { CommaOperatorTest(), ConstExprTest(), StrangeOperatorTest() };

int main(int argc, char *argv[]) {
  PlaygroundOrganizer po;

  po.Insert();
  po.PrintDetails();


  po.Run(po.All());

  return EXIT_SUCCESS;
}