#include "cpp_organizer.hpp"
#include "constexpr_test.hpp"

using namespace CppOrganizer;

class DummyTestClass : public CodeRunnerHelper<DummyTestClass>
{
public:
  DummyTestClass();
  ~DummyTestClass();

  void RunTest();

};

DummyTestClass::DummyTestClass() : CodeRunnerHelper("Sample Test Class")
{
}

DummyTestClass::~DummyTestClass()
{
}

void DummyTestClass::RunTest()
{
  std::cout << "Dummy Test Runned" << std::endl;
}


class CommaOperatorTest : public CodeRunnerHelper<CommaOperatorTest>
{
public:
  CommaOperatorTest() : CodeRunnerHelper("CommaOperatorTest") {}
  ~CommaOperatorTest() = default;

  void RunTest() {
    int x = (5, 4, 3, 2, 1, 0);
    printf("x = %d", x);
  }

};

std::vector<ICodeRunnerIdentifierPtr> CppOrganizer::_default_objects = 
{ 
  std::make_shared<CommaOperatorTest>(),
  std::make_shared<StrangeOperatorTest>(),
  std::make_shared<ConstExprTest>()
};

int main(int argc, char *argv[]) {
  PlaygroundOrganizer po;

  po.Insert();
  po.PrintDetails();

  po.RunAll();
  //po.RunWithID(po.All());

  return EXIT_SUCCESS;
}