// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 26/12/21.
//

#include "instrumentor.h"

Instrumentor::Instrumentor() : sessionName("None"), profileCount(0), activeSession(false) {
}

Instrumentor::~Instrumentor() {
  endSession();
}

Instrumentor &Instrumentor::Instance() {
  static Instrumentor instance;
  return instance;
}
// We can divide the program into multiple sessions with their name, placed on different json files
// The function just opens the file and writes the appropriate header for the Chrome tracer
void Instrumentor::beginSession(const std::string &name, const std::string &filepath) {
  if (activeSession) {
    endSession();
  }
  activeSession = true;
  outputStream.open(filepath);
  writeHeader();
  sessionName = name;
}

// Write appropriate footer for the Chrome tracer and closes file
void Instrumentor::endSession() {
  if (!activeSession) {
    return;
  }
  activeSession = false;
  writeFooter();
  outputStream.close();
  profileCount = 0;
}

void Instrumentor::writeProfile(const ProfileResult &result) {
  std::lock_guard<std::mutex> lock(memLock);
  if (profileCount++ > 0)
    outputStream << ",";

  std::string name = result.name;
  std::replace(name.begin(), name.end(), '"', '\'');

  outputStream << "{";
  outputStream << "\"cat\":\"function\",";
  outputStream << "\"dur\":" << (result.end - result.start) << ',';
  outputStream << "\"name\":\"" << name << "\",";
  outputStream << "\"ph\":\"X\",";
  outputStream << "\"pid\":0,";
  outputStream << "\"tid\":" << result.threadID << ",";
  outputStream << "\"ts\":" << result.start;
  outputStream << "}";

  // flush data every time so that if the application crashes, data was still recorded
  // outputStream.flush();
}

void Instrumentor::writeHeader() {
  outputStream << "{\"otherData\": {},\"traceEvents\":[";
  //outputStream.flush();
}

void Instrumentor::writeFooter() {
  outputStream << "]}";
  //outputStream.flush();
}

InstrumentationTimer::InstrumentationTimer(const std::string &name) : result({name, 0, 0, (std::thread::id) 0}),
                                                                      stopped(false) {
  //m_startTimepoint = std::chrono::high_resolution_clock::now();
  startTimePoint = std::chrono::steady_clock::now();
}

InstrumentationTimer::~InstrumentationTimer() {
  if (!stopped)
    stop();
}

void InstrumentationTimer::stop() {
  //auto endTimepoint = std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::steady_clock> endTimePoint = std::chrono::steady_clock::now();

  // start and end time in ms
  //long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch()
  // .count();
  //long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
  result.start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
  result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

  //trick to get the current thread id as an int:
  result.threadID = (std::thread::id) std::hash<std::thread::id>{}(std::this_thread::get_id());
  Instrumentor::Instance().writeProfile(result);

  stopped = true;
}