#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <queue>
#include <condition_variable>

template<typename T>
class Processor {
 private:
  std::thread internal_thread;
  std::queue<T> coda;
  bool closed;
  std::mutex m;
  std::condition_variable cv;
  T value;
  std::function<void(T)> f;
 public:
  Processor(std::function<void(T)> f) : closed(false) {
    internal_thread = std::move(std::thread([this, f]() {
      while (true) {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this]() { return !coda.empty() || closed; });
        if (closed && coda.empty())
          break;
        //       value = std::move(coda.front());
        value = coda.front();
        coda.pop();
        //ul.unlock();
        f(value);
        //ul.lock();
      }
    }));
    /*    std::thread t1([&]() {
          while (true) {
            std::unique_lock<std::mutex> ul(m);
            cv.wait(ul, [this]() { return !coda.empty() || closed; });
            if (closed)
              break;
            //       value = std::move(coda.front());
            value = coda.front();
            coda.pop();
            //ul.unlock();
            f(value);
            //ul.lock();
          }
        });
        internal_thread = std::move(t1);*/
  }

  ~Processor() {
    {
      std::unique_lock<std::mutex> ul(m);
      closed = true;
      cv.notify_one();
    }
    //release lock so that thread can exit from the wait and join.
    if (internal_thread.joinable())
      internal_thread.join();
  }

  void send(T t) {
    {
      std::unique_lock<std::mutex> ul(m);
      if (closed)
        throw std::runtime_error("Closed è già stata chiamata, nessuna f può più essere aggiunta");
      coda.push(t);
      cv.notify_one(); // nuovo elemento da processare, sveglia thread!
    }
  }

  void close() {

    std::unique_lock<std::mutex> ul(m);
    closed = true;
    cv.notify_one();
    // thread farà join nel distruttore!
  }
};

template<typename T>
void f(T t) {
  std::cout << t << std::endl;
}
int main() {
  std::thread t([]() {
    std::function<void(std::string)> f_display = f<std::string>;
    //f_display("ciao");
    Processor<std::string> p(f_display);
    p.send("ciao");
    p.send("urraaa1");
    p.send("urraaa2");
    p.send("urraaa3");
    p.send("urraaa4");
    p.send("urraaa5");
    p.close();
    //p.send("urraaa");
  });
  t.join();
  return 0;
}
