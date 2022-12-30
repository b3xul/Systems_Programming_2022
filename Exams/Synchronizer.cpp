// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 16/01/22.
//

#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>

//2 thread, ognuno legge un ingresso, quando ho 2 dati pronti li accoppia e li invia alla fase successiva
class Synchronizer {
 private:
  std::function<void(float d1, float d2)> next_phase;
  float d1;
  bool d1_available;
  float d2;
  bool d2_available;
  bool entering;
  std::mutex m;
  std::condition_variable cv;
 public:
  Synchronizer(std::function<void(float d1, float d2)> process)
      : next_phase(process), d1_available(false), d2_available(false), entering(true) {
  }
  void dataFromFirstPort(float d1) {
    // attendi che arriva l'altro dato. Quando arriva sbloccati.
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul, [this]() { return entering; });

    this->d1 = d1;
    d1_available = true;
    if (d2_available) {
      //abbiamo entrambi i dati
      entering = false;
      next_phase(d1, d2);
      cv.notify_one(); //l'altro si sveglierà ma questo avrà già chiamato la funzione successiva, quindi dovrà solo
      // pulire i dati e uscire
      return;
    }
    // non abbiamo entrambi i dati
    cv.wait(ul, [this]() { return (d1_available && d2_available); });
    //arriva dato, l'altra porta ha già mandato alla fase successiva
    d1_available = false;
    d2_available = false;
    entering = true;
    cv.notify_one();
  }
  void dataFromSecondPort(float d2) {
    // attendi che arriva l'altro dato. Quando arriva sbloccati.
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul, [this]() { return entering; });

    this->d2 = d2;
    d2_available = true;
    if (d1_available) {
      //abbiamo entrambi i dati
      entering = false;
      next_phase(d1, d2);
      cv.notify_one(); //l'altro si sveglierà ma questo avrà già chiamato la funzione successiva, quindi dovrà solo
      // pulire i dati e uscire
      return;
    }
    // non abbiamo entrambi i dati
    cv.wait(ul, [this]() { return (d1_available && d2_available); });
    //arriva dato, l'altra porta ha già mandato alla fase successiva
    d1_available = false;
    d2_available = false;
    entering = true;
    cv.notify_one();

  }
};

int main() {

  Synchronizer s([](float d1, float d2) {
    std::cout << "Elaboro dati: " << d1 << " e " << d2 << std::endl;
  });
  std::thread t1([&s]() {

    for (int i = 1; i < 50; i++) {
      s.dataFromFirstPort(static_cast<float>(i) * 2.5);
    }
  });
  std::thread t2([&s]() {

    for (int i = 1; i < 50; i++) {
      s.dataFromSecondPort(static_cast<float>(i));
    }
  });
  t1.join();
  t2.join();
}