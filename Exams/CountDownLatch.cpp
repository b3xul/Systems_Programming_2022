// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 15/01/22.
//

#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <queue>
#include <condition_variable>

class CountDownLatch {
  //sincronizzare 1 o + thread che devono attendere senza consumare cpu (condition_variable) il completamento di
  // operazioni in corso in altri thread.

 private:
  int counter;
  std::mutex m; //mutex su counter, condiviso tra più thread che provano ad accedere a counter
  std::condition_variable cv; // condition variable che avvisa quando counter diventa=0
 public:
  CountDownLatch(int N) {
    if (N > 0)
      this->counter = N;
    else
      throw std::logic_error("Contatore deve essere positivo");
  }
  void await() {
    std::unique_lock<std::mutex> ul(m);
    if (counter == 0)
      return;
    cv.wait(ul, [this]() { return counter == 0; })
    return;
  }

  void countDown() {
    std::unique_lock<std::mutex> ul(m); //lavoro su counter, serve prendere il lock
    if (counter > 0) {
      counter--;
      if (counter == 0)
        cv.notify_all();
    }

    return;
  }
};