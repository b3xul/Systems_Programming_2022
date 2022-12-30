//https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e

#ifndef INSTRUMENTOR_H_
#define INSTRUMENTOR_H_

//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().beginSession("Session name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().endSession();                        // end Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>
#include <mutex>

#define PROFILING 1
#if PROFILING

//https://coderedirect.com/questions/52992/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define SCOPE_LINE_NUMBER TOKENPASTE2(scope_line_, __LINE__)

#define PROFILE_SCOPE(name) InstrumentationTimer SCOPE_LINE_NUMBER(name) // used for scopes < 1 entire function
#define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__) // used to profile an entire function without the need to pass
// the name, and disambiguating between the different overloading functions (__PRETTY_FUNCTION__ is similar to
// __FUNCSIG__) and contains the entire signature of the function!
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

struct ProfileResult {
  const std::string name;
  long long start, end;
  std::thread::id threadID; // typedef unsigned long int -> pthread_t -> __gthread_t -> native_handle_type _M_thread;
};

class Instrumentor {
 private:
  std::string sessionName;
  std::ofstream outputStream;
  int profileCount;
  std::mutex memLock;
  bool activeSession;

 public:
  Instrumentor();
  ~Instrumentor();

  static Instrumentor &Instance();

  // We can divide the program into multiple sessions with their name, placed on different json files
  // The function just opens the file and writes the appropriate header for the Chrome tracer
  void beginSession(const std::string &name, const std::string &filepath = "results.json");

  // Write appropriate footer for the Chrome tracer and closes file
  void endSession();

  void writeProfile(const ProfileResult &result);

  void writeHeader();

  void writeFooter();
};

class InstrumentationTimer {
 private:
  ProfileResult result;
  //std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
  std::chrono::time_point<std::chrono::steady_clock> startTimePoint;
  bool stopped;
 public:
  InstrumentationTimer(const std::string &name);
  ~InstrumentationTimer();

  void stop();
};

#endif //INSTRUMENTOR_H_
