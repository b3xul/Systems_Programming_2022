// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 15/01/22.
//

#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cassert>

template<typename T>
void f(T t){

}
class Joiner {
 private:
  int count;
  int N;
  std::map<std::thread::id, double> values;
  std::mutex m;
  std::condition_variable cv;
  bool entering;
 public:
  Joiner(int N) : N(N), count(0), entering(true) {
  }
  std::map<std::thread::id, double> supply(double value) {
    // qualcuno ha un dato pronto
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul, [this]() { return entering; }); //se sono in fase di ingresso posso entrare

    //,[this](){ return this->can_add_value(); }
    values[std::this_thread::get_id()] = value;
    count++;
    if (count < N) {
      //aspetto finchè non ci sono tutte e N le misurazioni
      cv.wait(ul, [this]() { return !entering; });//è ora di uscire!
    } else {
      //questo thread ha aggiunto l'ultima misurazione
      cv.notify_all(); // ehi tutti, svegliatevi, l'ultima misurazione è stata aggiunta! N.B. non si svegliano qui,
      // ma dopo il return, perchè fino ad allora il mutex ce l'ho io!!
      entering = false;
    }
    count--; // l'ultimo che esce libera la mappa!
    if (count == 0) {
      entering = true;
      cv.notify_all(); // quando l'ultimo finisce, tutti quelli che erano in attesa di entrare vengono notificati, si
      // svegliano, vedono entering=true e iniziano ad attendere il possesso di ul, che potranno contendersi appena
      // l'ultimo ha fatto return.
      std::map<std::thread::id, double> tmp(values);
      values.clear();

      return tmp;
    }
    return values;
  }
};

int main() {
  const int N = 4;
  Joiner joiner(N);
  int iter = 1000;
  auto f = [&joiner, N, iter](bool print) {
    for (int i = 0; i < iter; i++) {
      auto m = joiner.supply(i);
      if (m.size() != N)
        std::cout << "expected " << N << " found " << m.size() << std::endl;
      assert(m.size() == N);
      for (auto p: m) {
        if (p.second != i)
          std::cout << "expected " << i << " found " << p.second << std::endl;
        assert(p.second == i);
      }
      if (print) {
        std::cout << i << " ";
        if (i % 10 == 9)
          std::cout << std::endl;
      }
    }
  };
  std::thread threads[N];
  for (int i = 0; i < N; i++) {
    threads[i] = std::thread(f, i == 0);
  }
  for (auto &thread: threads)
    thread.join();
}