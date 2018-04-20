#include <cpp_organizer.h>
#include "constexpr_test.hpp"
#include "FutureExample.hpp"
#include <Windows.h>
#include <iostream>

using namespace CppOrganizer;

START_ORGANIZER_MAIN_WITH_OBJS(
{
  ADD_ELEMENT(ConstExprTest),
  ADD_ELEMENT(FutureUsage),
})
PAUSE_ON_END;
END_ORGANIZER_MAIN;
