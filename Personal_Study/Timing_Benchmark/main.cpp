//
// Created by bexul on 24/12/21.
//

// N.B. For other simpler strategies see in Lab1 DurationLogger class.
#include <iostream>
#include <memory>

#include <chrono>

class DurationLogger {
 public:
 DurationLogger(const std::string &name)
      : name(name), clockI(clock()), timeI(time(NULL)), chronoI(std::chrono::steady_clock::now()) {
    std::cout << ">>>>>>>>>>>>>> starting [ " << name << " ] at clock[ " << clockI << " ] at time[ " << timeI << " "
                                                                                                                 "]\n";

  }
  ~DurationLogger() {
    Stop();
  }
  void Stop() {
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

//    std::chrono::time_point<std::chrono::steady_clock> endTimePoint = std::chrono::steady_clock::now();
//    // start and end time in ms
//    long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
//    long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
//
//    long duration = end - start; // duration in microseconds
//    double ms = duration * 0.001;
//    double s = ms * 0.001;
//
//    std::cout << name << ": " << duration << "us - " << ms << "ms - " << s << "s\n";
  }
 private:
  std::string name;
  clock_t clockI;
  time_t timeI;
  std::chrono::time_point<std::chrono::steady_clock> chronoI;
};

int main() {
  int value = 0;
  {
    DurationLogger t("forThisScope");
    for (int i = 0; i < 10000000; i++)
      value += 2;
  }

  struct Vector2 { float x, y; };
  {
    std::array<std::unique_ptr<Vector2>, 1000> uniquePtrs;
    DurationLogger t("uniquePtrDuration");
    for (int i = 0; i < uniquePtrs.size(); i++)
      uniquePtrs[i] = std::make_unique<Vector2>();
  }
  {
    std::array<std::shared_ptr<Vector2>, 1000> sharedPtrs;
    DurationLogger t("makeSharedDuration");
    for (int i = 0; i < sharedPtrs.size(); i++)
      sharedPtrs[i] = std::make_shared<Vector2>();
  }
  {
    std::array<std::shared_ptr<Vector2>, 1000> sharedPtrs;
    DurationLogger t("newSharedPtrDuration");
    for (int i = 0; i < sharedPtrs.size(); i++)
      sharedPtrs[i] = std::shared_ptr<Vector2>(new Vector2());
  }

  // N.B. ALWAYS BENCHMARK IN RELEASE!
  // RESULTS WITH 100000 ELEMENTS:
  // IN RELEASE: make_unique[primo] 1.408ms, make_shared[secondo] 3.524ms, new_shared[terzo] 4.81ms
  // IN DEBUG  : make_unique[secondo] 23.408ms, make_shared[terzo] 28.857ms, new_shared[primo] 20.803ms

  // RESULTS WITH 1000 ELEMENTS:
  // IN RELEASE: make_unique[secondo] 0.026ms, make_shared[primo] 0.01ms, new_shared[terzo] 0.038ms
  // IN DEBUG  : stesso ordine di prima

  // Basandosi solo sulla configurazione in debug, verrebbe scelto new_shared, quando invece make_shared e' sempre
  // piu' veloce di new_shared in release!!
}
