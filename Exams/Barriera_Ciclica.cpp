// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 15/01/22.
//

#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

class Barriera_Ciclica {
 private:
  int count;
  std::mutex m;
  std::condition_variable cv;
  int N;
  bool entering;
 public:
  Barriera_Ciclica(int N) : N(N), count(0), entering(true) {
  }

  void attendi(int i) {
    std::unique_lock<std::mutex> ul(m); //prova a prendere il mutex

    cv.wait(ul, [this]() { return entering; }); // se non sono ancora a N si può entrare
    std::cout << i << " e' entrato" << std::endl;
    count++;
    if (count < N) {
      cv.wait(ul, [this]() { return !entering; }); // attendo che si arrivi a N
    } else {
      std::cout << i << " e' l'N-esimo thread" << std::endl;
      //quando arrivo a N metto in attesa altri che vogliono entrare e faccio ritornare gli altri
      entering = false;
      cv.notify_all();
    }
    count--;
    if (count == 0) {
      std::cout << i << " e' l'ultimo thread rimasto" << std::endl;
      //l'ultimo che esce fa rientrare quelli nuovi
      entering = true;
      cv.notify_all();
    }
    return;
  }
};

int main() {
  const int N = 5;
  Barriera_Ciclica bc(N);
  auto f = [&bc, N](int i) {
    bc.attendi(i);
  };

  std::thread threads[N* 10 ];
  for (int i = 0; i < N* 10; i++) {
    threads[i] = std::thread(f, i);
  }
  for (auto &thread: threads)
    thread.join();
}