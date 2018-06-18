#include "playground_organizer.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <future>
#include <chrono>
#include <thread>

namespace asyncusage {
  using namespace std::chrono_literals;
  template<typename T, int N>
  struct CompileTimeArray {
    constexpr CompileTimeArray() : arr(), len(N - 1), endIndex(N - 1) {
      while (len > 0)
      {
        arr[len] = len;
        len = len - 1;
      }
    }

    constexpr T operator[] (std::size_t index) const {
      return arr[index];
    }

    T arr[N];
    std::size_t endIndex;
  private:
    int len;
  };

  template <typename RandomIt, size_t AsyncThreshold = 1000>
  int parallel_sum(RandomIt beg, RandomIt end)
  {
    std::cout << "running thread id: " << std::this_thread::get_id() << std::endl;
    auto len = end - beg;
    if (len < AsyncThreshold) {
      return std::accumulate(beg, end, 0, [](auto x, auto y) {
        //return after some delay.
        std::this_thread::sleep_for(1ms); // sleep 1 millisecond for every calculation
        return x + y;
      });
    }

    RandomIt mid = beg + len / 2;
    auto handle = std::async(std::launch::async,
                             parallel_sum<RandomIt>, mid, end);
    int sum = parallel_sum(beg, mid);
    return sum + handle.get();
  }

} // namespace asyncusage 

CREATE_ELEMENT_WITH_CODE(AsyncUsageExample) {
  using namespace asyncusage;
  // list generated at compile time.
  constexpr auto list = CompileTimeArray<int, 2000>();
  auto sum = parallel_sum(&list.arr[0], &list.arr[list.endIndex]);
  std::cout << "Sum of array: " << sum << std::endl;
}

CREATE_ELEMENT_WITH_CODE(AsyncSequentialExample) {
  using namespace asyncusage;
  // list generated at compile time.
  constexpr auto list = CompileTimeArray<int, 2000>();
  auto sum = parallel_sum<decltype(&list.arr[0]), 10001>(&list.arr[0], &list.arr[list.endIndex]);
  std::cout << "Sum of array: " << sum << std::endl;
}

START_ORGANIZER_MAIN_WITH_OBJS(
{
  ADD_ELEMENT(AsyncSequentialExample),
  ADD_ELEMENT(AsyncUsageExample),
})
PAUSE_ON_END;
END_ORGANIZER_MAIN;