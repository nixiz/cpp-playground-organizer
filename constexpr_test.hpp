#pragma once

#include "cpp_organizer.hpp"

namespace CppOrganizer
{
  constexpr int factorial(int n)
  {
    return n <= 1 ? 1 : (n * factorial(n - 1));
  }

  int mutliplyWithDouble(int i) {
    return i * 2;
  }

  class ConstExprTest : public CodeRunnerHelper<ConstExprTest>
  {
  public:
    ConstExprTest();
    ~ConstExprTest();

    void Run() {
      int i = factorial(5);
      int ii = factorial(mutliplyWithDouble(5));
    }

  private:

  };

  ConstExprTest::ConstExprTest() : CodeRunnerHelper("ConstExprTest")
  {
  }

  ConstExprTest::~ConstExprTest()
  {
  }

  class StrangeOperatorTest : public CodeRunnerHelper<StrangeOperatorTest>
  {
  public:
    StrangeOperatorTest();
    ~StrangeOperatorTest();

    void Run() {
      int x = 10;
      while (x--> 0) // x goes to 0
      {
        printf("%d ", x);
      }
    }
  private:

  };

  StrangeOperatorTest::StrangeOperatorTest() : 
    CodeRunnerHelper("Strange Operator \"-->\" Test")
  {

  }

  StrangeOperatorTest::~StrangeOperatorTest()
  {
  }
}

