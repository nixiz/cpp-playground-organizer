#pragma once
#include <cpp_organizer.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <future>
#include <iostream>

namespace CppOrganizer
{

  class DummyClass
  {
  public:
    // object member binding
    int ReturnAfterOneSecond(int i) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      return i;
    }
  };


  // global method sample
  int ReturnAfterTwoSecond(int i) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return i;
  }


  class FutureUsage : public CodeRunnerHelper<FutureUsage>
  {
  public:
    FutureUsage() : CodeRunnerHelper("FutureUsage") {
    }
   
    ~FutureUsage() = default;

    void Run() {

      // future from an async()
      std::future<int> f1 = std::async(std::launch::async, []() { return 5; });
      // 
      std::cout << "Done!\nResult is: " << f1.get() << '\n';

      DummyClass d;

      // start async calculation with object member function binding. 
      std::future<int> f2 = std::async(std::launch::async, std::bind(&DummyClass::ReturnAfterOneSecond, d, std::placeholders::_1), 5);

      // waits for async call end.
      f2.wait();

      std::cout << "Done!\nResult is: " << f2.get() << '\n';


      // start async calculation with global function binding. 
      std::future<int> f3 = std::async(std::launch::async, ReturnAfterTwoSecond, 5);

      // get() also waits for async call end.
      std::cout << "Done!\nResult is: " << f3.get() << '\n';
    }
  };

} // namespace