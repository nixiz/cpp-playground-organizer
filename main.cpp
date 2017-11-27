#include "cpp_organizer.hpp"
#include "constexpr_test.hpp"

using namespace CppOrganizer;

class DummyTestClass : public CodeRunnerHelper<DummyTestClass>
{
public:
  DummyTestClass();
  ~DummyTestClass();

  void Run();

};

DummyTestClass::DummyTestClass() : CodeRunnerHelper("Sample Test Class")
{
}

DummyTestClass::~DummyTestClass()
{
}

void DummyTestClass::Run()
{
  std::cout << "Dummy Test Runned" << std::endl;
}


class CommaOperatorTest : public CodeRunnerHelper<CommaOperatorTest>
{
public:
  CommaOperatorTest() : CodeRunnerHelper("CommaOperatorTest") {}
  ~CommaOperatorTest() = default;

  void Run() {
    int x = (5, 4, 3, 2, 1, 0);
    printf("x = %d", x);
  }

};

class OperatorNonOverload : public CodeRunnerHelper<OperatorNonOverload>
{
public:
  OperatorNonOverload() : CodeRunnerHelper("OperatorNonOverload") {}
  ~OperatorNonOverload() = default;
#ifndef COMPILE_WITH_UNIMPLEMENTED_RUNNER
  void Run() {
    printf("%s", __FUNCTION__);
  }
#endif
};


std::vector<ICodeRunnerIdentifierPtr> CppOrganizer::_default_objects = 
{ 
  std::make_shared<CommaOperatorTest>()
  ,std::make_shared<StrangeOperatorTest>()
  ,std::make_shared<ConstExprTest>()
  ,std::make_shared<OperatorNonOverload>()
};

int main(int argc, char *argv[]) {
  PlaygroundOrganizer po;

  po.Insert();
  po.PrintDetails();

  po.RunAll();
  //po.RunWithID(po.All());

  return EXIT_SUCCESS;
}