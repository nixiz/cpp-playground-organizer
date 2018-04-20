#pragma once

#include <cpp_organizer.h>

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
    void Run() {
      int i = factorial(5);
      int ii = factorial(mutliplyWithDouble(5));
    }
  };

}

