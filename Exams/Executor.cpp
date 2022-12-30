// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 15/01/22.
//

#include <iostream>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>
#include <future>

class Context {
  std::string s = "default";
 public:
  Context(std::string string) : s(string) {
  }
  void print() {
    std::cout << s << std::endl;
  }
};

class Executor {
 private:
  std::thread internal_thread;
  Context *ctx;
  bool already_shutdown; //più thread devono accedere contemporaneamente a questo, serve mutex
  std::queue<std::packaged_task<void(Context *)>> jobs; //per ritornare future è comodo usare packaged_task
  std::mutex m;
  std::condition_variable cv;

 public:
  Executor(Context *ctx) : ctx(ctx), already_shutdown(false) {
    internal_thread = std::move(std::thread([this]() {
      std::cout << "internal thread started" << std::endl;
      //questo thread rimane in attesa che arrivino nuove funzioni in coda da svolgere
      while (true) {
        //si esce dal while quando si chiama shutdown o il distruttore, che rendono already_shutdown true
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this]() {
          return (!jobs.empty() || (already_shutdown && jobs.empty()));
        });
        std::cout << "thread è stato avvisato che ora jobs è empty e si è fatto lo shutdown oppure che ora c'è "
                     "qualcosa di nuovo da processare E m non è più locked -> thread si sveglia ed ha il lock"
                  << std::endl;

        if (jobs.empty() && already_shutdown)
          //non c'è più niente da fare -> thread finisce (join)
          break;

        //c'è ancora da fare
        //estrai dalla coda e svolgi lavoro
        //auto job_to_do = std::move(jobs.front());
        std::packaged_task<void(Context *)> job_to_do = std::move(jobs.front());
        jobs.pop();
        std::cout << "thread lancia f" << std::endl;
        ul.unlock();
        job_to_do(this->ctx);
      }
    }));
  }
  ~Executor() {
    {
      std::lock_guard<std::mutex> lg(m);
      if (!already_shutdown) {
        already_shutdown = true;
        cv.notify_one();
      }
      std::cout << "internal_thread ended" << std::endl;
      std::cout << "queue.size()=" << jobs.size() << std::endl;
    } //essential to release this lock!!!
    internal_thread.join();
    std::cout << "internal_thread really ended" << std::endl;
  }
  std::future<void> submit(std::function<void(Context *)> f) {
    std::unique_lock<std::mutex> ul(m);
    if (already_shutdown)
      throw std::logic_error("Already shut down");

    std::packaged_task<void(Context *)> p_t(f);
    std::future<void> future_result = p_t.get_future();
    jobs.push(std::move(p_t));
    std::cout << "Inserita nuova funzione" << std::endl;

    cv.notify_one(); //unlock thread which is waiting for the queue to be filled

    return future_result;
  }

  void shutdown() {
    std::unique_lock<std::mutex> ul(m);
    already_shutdown = true;
    ul.unlock();
    cv.notify_one();
  }
};

void func(Context *c) {
  c->print();
}

int main() {
  Context *c = new Context("CONTESTO");
  Executor exec(c);

  std::future<void> f_a;
  f_a = exec.submit([](Context *c) {
    //std::this_thread::sleep_for(std::chrono::seconds(5));
    c->print();
  });
  //f_a.get(); non necessario prendere il future.

  //std::this_thread::sleep_for(std::chrono::seconds(3));
  f_a = exec.submit([](Context *c) {
    //std::this_thread::sleep_for(std::chrono::seconds(3));
    c->print();
  });
  f_a.get();
  f_a = exec.submit([](Context *c) {
    //std::this_thread::sleep_for(std::chrono::seconds(10));
    c->print();
  });
  f_a.get();
  f_a = exec.submit(func);
  f_a.get();
  exec.shutdown();
  /*  std::thread t([]() {
      *//*  std::thread internal_thread;
      internal_thread = std::move(std::thread([]() {
        std::cout << "ciao" << std::endl;
      }));
      internal_thread.join();*//*

    std::function<void(Context *)> f;

    Context *a;
    Executor b(a);
    b.submit(f);
    b.submit(f);
    b.submit(f);
    b.submit(f);
    b.submit(f);
    b.submit(f);

    b.shutdown();

  });
  t.join();*/
}