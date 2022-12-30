//
// Created by bexul on 24/12/21.
//

// N.B. For other simpler strategies see in Lab1 DurationLogger class.
#include <iostream>
#include <memory>
#include <thread>

#include <chrono>
#include "instrumentor.h"

void FuncTest() {
  PROFILE_FUNCTION();
  for (int i = 0; i < 2000; i++) {
    std::cout << "Hello!\n";
  }
}
void FuncTest(int n) {
  PROFILE_FUNCTION();
  for (int i = 0; i < n; i++) {
    std::cout << "Hello!\n";
  }
}
void twoFunctTest() {
  PROFILE_FUNCTION();
  FuncTest();
  FuncTest(5000);
  std::thread a([]() { FuncTest(); });
  std::thread b([]() { FuncTest(5000); });

  a.join();
  b.join();
}

int main() {
  Instrumentor::Instance().beginSession("Profile1");
  int value = 0;
  {
    PROFILE_SCOPE("forThisScope");
    for (int i = 0; i < 1000000; i++)
      value += 2;
  }

  struct Vector2 { float x, y; };
  {
    std::array<std::unique_ptr<Vector2>, 100000> uniquePtrs;
    PROFILE_SCOPE("uniquePtrDuration");
    for (int i = 0; i < uniquePtrs.size(); i++)
      uniquePtrs[i] = std::make_unique<Vector2>();
  }
  {
    std::array<std::shared_ptr<Vector2>, 100000> sharedPtrs;
    PROFILE_SCOPE("makeSharedDuration");
    for (int i = 0; i < sharedPtrs.size(); i++)
      sharedPtrs[i] = std::make_shared<Vector2>();
  }
  {
    std::array<std::shared_ptr<Vector2>, 100000> sharedPtrs;
    PROFILE_SCOPE("newSharedPtrDuration");
    for (int i = 0; i < sharedPtrs.size(); i++)
      sharedPtrs[i] = std::shared_ptr<Vector2>(new Vector2());
  }
  twoFunctTest();
  Instrumentor::Instance().endSession();

  // {"otherData": {},"traceEvents":[
  // {"cat":"function","dur":22483,"name":"forThisScope","ph":"X","pid":0,"tid":0,"ts":22490042555} ,
  // {"cat":"function","dur":264,"name":"uniquePtrDuration","ph":"X","pid":0,"tid":0,"ts":22490065167} ,
  // {"cat":"function","dur":293,"name":"makeSharedDuration","ph":"X","pid":0,"tid":0, "ts":22490065547} ,
  // {"cat":"function","dur":196,"name":"newSharedPtrDuration","ph":"X","pid":0,"tid":0,"ts":22490066042}
  // ]}
}
