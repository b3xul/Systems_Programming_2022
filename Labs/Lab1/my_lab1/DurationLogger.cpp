//
// Created by bexul on 09/04/21.
//

#include "DurationLogger.h"

DurationLogger::DurationLogger(const std::string &name)
    : name(name), clockI(clock()), timeI(time(NULL)), chronoI(std::chrono::steady_clock::now()) {
  std::cout << ">>>>>>>>>>>>>> starting [ " << name << " ] at clock[ " << clockI << " ] at time[ " << timeI << " "
                                                                                                               "]\n";

}
DurationLogger::~DurationLogger() {
  clock_t clockF = clock();
  time_t timeF = time(NULL);
  std::chrono::time_point<std::chrono::steady_clock> chronoF = std::chrono::steady_clock::now();
  double clockDuration = (double(clockF - clockI)) / CLOCKS_PER_SEC;
  double timeDuration = difftime((int) timeF, (int) timeI);
  std::chrono::duration<double> chronoDuration = chronoF - chronoI; // in s : 0.00268433s
  //  std::chrono::duration<, std::milli> chronoDuration = chronoF - chronoI; // in ms : 2.68433ms
  std::chrono::duration<double>
      chronoDuration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(chronoDuration); // in s with
  // milliseconds precision: 0.002s
  std::cout << "<<<<<<<<<<<<<< ending [ " << name << " ] at clock[ " << clockF << " ] after [" << clockDuration
            << " seconds] at time[ " << timeF << " ] after [" << timeDuration << " seconds] after ["
            << chronoDuration.count() << "ms = " << chronoDuration_ms.count() << "s]\n";

}
