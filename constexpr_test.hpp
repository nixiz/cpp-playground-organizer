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

  class ConstExprTest : public PlaygroundHelper<ConstExprTest>
  {
  public:
    ConstExprTest();
    ~ConstExprTest();

    void RunTest() {
      int i = factorial(5);
      int ii = factorial(mutliplyWithDouble(5));
    }

  private:

  };

  ConstExprTest::ConstExprTest() : PlaygroundHelper("ConstExprTest")
  {
  }

  ConstExprTest::~ConstExprTest()
  {
  }

  class StrangeOperatorTest : public PlaygroundHelper<StrangeOperatorTest>
  {
  public:
    StrangeOperatorTest();
    ~StrangeOperatorTest();

    void RunTest() {
      int x = 10;
      while (x--> 0) // x goes to 0
      {
        printf("%d ", x);
      }
    }
  private:

  };

  StrangeOperatorTest::StrangeOperatorTest() : 
    PlaygroundHelper("Strange Operator \"-->\" Test")
  {

  }

  StrangeOperatorTest::~StrangeOperatorTest()
  {
  }
}

