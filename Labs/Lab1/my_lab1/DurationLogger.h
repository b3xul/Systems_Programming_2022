//
// Created by bexul on 09/04/21.
//

#ifndef LAB1_P_ES1_DURATIONLOGGER_H_
#define LAB1_P_ES1_DURATIONLOGGER_H_

#include <iostream> //size_t
#include <ctime> //clock_t, time_t
#include <chrono> //

class DurationLogger {
 public:
  explicit DurationLogger(const std::string &name);
  ~DurationLogger();
 private:
  std::string name;
  clock_t clockI;
  time_t timeI;
  std::chrono::time_point<std::chrono::steady_clock> chronoI;
  //The high_resolution_clock is not implemented consistently across different standard library implementations, and
  // its use should be avoided. It is often just an alias for std::chrono::steady_clock or std::chrono::system_clock,
  // but which one it is depends on the library or configuration. When it is a system_clock, it is not monotonic (e.g
  // ., the time can go backwards). For example, for gcc's libstdc++ it is system_clock, for MSVC it is steady_clock,
  // and for clang's libc++ it depends on configuration.
  // Generally one should just use std::chrono::steady_clock or std::chrono::system_clock directly instead of
  // std::chrono::high_resolution_clock: use steady_clock for duration measurements, and system_clock for wall-clock time.

  //N.B.
  // Refuse to use count() and time_since_epoch() except for I/O!
  // Refuse to use conversion factors (such as 1000).
};

#endif //LAB1_P_ES1_DURATIONLOGGER_H_
