//
// Created by bexul on 14/01/22.
//

#ifndef ES1_ES1_JOBS_HPP_
#define ES1_ES1_JOBS_HPP_

#include <queue>
#include <mutex>

std::mutex queue_access;
std::mutex files_available;
std::condition_variable files_available_cv;
std::mutex lines_available;
std::condition_variable lines_available_cv;
template<class T>
class Jobs {
 private:
  std::queue<T> jobs;
  std::size_t MAX_SIZE = 50;
 public :

  // inserisce un job in coda in attesa di essere processato , puo ’
  // essere bloccante se la coda dei job e’ piena
  void put(T job) {

    std::unique_lock ul(queue_access);
    this->jobs.emplace(job);
    /*if (this->jobs.size() == 1) {
            files_available.unlock();
            files_available_cv.notify_one();
    }*/
  }
  // legge un job dalla coda e lo rimuove
  // si blocca se non ci sono valori in coda
  T get() {
    //std::unique_lock     l(queue_access); m mutex per proteggere l'accesso al contenitore e impedire che tutti i
    // thread ci accedano contemporaneamente!

    std::unique_lock ul(queue_access);
    T res = this->jobs.front();
    this->jobs.pop();
    /*    if (this->jobs.size() == 0)
          files_available.lock();*/
    return res;
  }

  bool empty() {
    std::unique_lock ul(queue_access);

    bool res = this->empty();
    return res;
  }

  std::size_t size() {
    std::unique_lock ul(queue_access);
    std::size_t res = this->jobs.size();
    return res;
  }
  std::size_t max_size() {
    std::unique_lock ul(queue_access);

    std::size_t res = this->MAX_SIZE;

    return res;
  }
  bool can_produce() {
    std::unique_lock ul(queue_access);

    bool res = (this->jobs.size() < this->MAX_SIZE); //&& (this->jobs.size() != 0)

    return res;
  }
  bool can_consume() {
    std::unique_lock ul(queue_access);

    bool res = (this->jobs.size() > 0);

    return res;
  }
};

#endif //ES1_ES1_JOBS_HPP_
