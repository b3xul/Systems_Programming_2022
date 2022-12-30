// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <vector>
#include <future>
#include <filesystem>
#include <functional>
#include <fstream>

#include "../../../Personal_Study/Timing_Visual_Profiler/instrumentor.h"
/*bool directory_completed = false;
std::mutex directory_completed_m;*/
std::atomic<bool> directory_completed = false;
std::mutex cout_stream;

#include "Jobs.hpp"

// serve struttura condivisa thread-safe a cui si possano appoggiare producers and consumers
/*Jobs<std::packaged_task<void(std::string)>> fileJobs;
Jobs<std::packaged_task<void(std::string)>> lineJobs;*/
Jobs<std::string> fileJobs;
Jobs<std::tuple<std::string, std::string>> lineJobs;

std::string directory_path = "./../Files";
std::string regex = "ciao";

void consumer(int i) {
  PROFILE_FUNCTION();
  while (lineJobs.can_consume() || fileJobs.can_produce()) {
    /*    cout_stream.lock();
        std::cout << "Executing line producer " << i << std::endl;
        cout_stream.unlock();*/
    /*    bool to_exit = false;
        directory_completed_m.lock();
        if (directory_completed)
          to_exit = true;
        directory_completed_m.unlock();

        if (to_exit)
          break;*/
    std::unique_lock<std::mutex> lines_available_ul(lines_available); // while it is producing, it has the lock
    {
      PROFILE_SCOPE("waitingLines");
      lines_available_cv.wait(lines_available_ul, []() {
        /*        bool to_exit = false;
                directory_completed_m.lock();
                if (directory_completed)
                  to_exit = true;
                directory_completed_m.unlock();
                return (lineJobs.can_consume() || to_exit);*/
        return (lineJobs.can_consume() || directory_completed.load());
      });
    }
    /*      if (lineJobs.size() == 0)
            break;*/
    /*    cout_stream.lock();
        std::cout << "Line producer " << i << " is going to produce lines, since data is available" << std::endl;
        cout_stream.unlock();*/

    /*    bool to_exit = false;
        directory_completed_m.lock();
        if (directory_completed)
          to_exit = true;
        directory_completed_m.unlock();
        if (to_exit)
          break;*/
    if (directory_completed.load())
      break;

    std::string filename;
    std::string line;
    std::tie(filename, line) = lineJobs.get();
    if (line.find(regex) != std::string::npos) {
      cout_stream.lock();
      std::cout << "Found in " << filename << " at line: " << line << std::endl;
      cout_stream.unlock();
    }

    cout_stream.lock();
    std::cout << "Line consumer [" << i << "] reduced lineJobs to the current size: " << lineJobs.size() << std::endl;
    cout_stream.unlock();

    lines_available_ul.unlock(); // cosi' altri consumer possono prenderlo
    lines_available_cv.notify_one();
  }
  cout_stream.lock();
  std::cout << "Line consumer (" << i << ") ha terminato la sua esecuzione" << std::endl;
  cout_stream.unlock();
  return;
}

/*void producer(int i) {
  // puo' generare numero arbitrario finito di informazioni in un tempo arbitrario. Ogni informazione generata deve
  // essere elaborata da uno ed un solo consumer.
  std::unique_lock<std::mutex> ul(files_available);
  while (directory_completed.try_lock() == false) {
    //still new data is being added to fileJobs queue
    files_available_cv.wait(ul);
  }
  *//*  tmp = root;
    for (const std::filesystem::path &element: it.path()) {
      if (element != "/") {
        tmpS = tmp.lock();
        next = tmpS->get(element);
        if (next == nullptr) {
          //directory/file not present
          if (it.is_directory()) {
            tmp = tmpS->addDirectory(element);
          } else if (it.is_regular_file())
            tmpS->addFile(element, std::filesystem::file_size(it));
        } else if (tmpS->myType() == 0) //directory already exists
          tmp = std::dynamic_pointer_cast<Directory>(next);
      }
    }

    int res = consumer(v.begin(), v.end());

    mtx.lock();
    std::cout << "Producer: " << i << " result: " << res << std::endl;
    mtx.unlock();*//*

  //cout stream non e' thread safe!
  // Producer: Producer: 21 result:  result: 1000010000
  //
  //Producer: 6 result: 10000
}*/

void lines_producer(int i) {
  PROFILE_FUNCTION();
  while (fileJobs.can_produce() && fileJobs.size() != 0) {
    /*    cout_stream.lock();
        std::cout << "FileJobs current size: " << fileJobs.size() << std::endl;
        cout_stream.unlock();*/
    /*    cout_stream.lock();
        std::cout << "Executing line producer " << i << std::endl;
        cout_stream.unlock();*/
    std::unique_lock<std::mutex> files_available_ul(files_available); // while it is producing, it has the lock
    {
      PROFILE_SCOPE("waitingFiles");
      files_available_cv.wait(files_available_ul, []() { return (fileJobs.can_produce() || fileJobs.size() == 0); });
    }
    if (fileJobs.size() == 0)
      break;
    /*    cout_stream.lock();
        std::cout << "Line producer " << i << " is going to produce lines, since data is available" << std::endl;
        cout_stream.unlock();*/
    std::string filename = fileJobs.get();
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
      std::unique_lock<std::mutex> lines_available_ul(lines_available); // while it is producing, it has the lock
      lines_available_cv.wait(lines_available_ul, []() { return (lineJobs.can_produce()); });

      cout_stream.lock();
      std::cout << "Line producer (" << i << ") legge file {" << filename << "} e produce stringa [" << line << ']'
                << std::endl;
      cout_stream.unlock();
      lineJobs.put(std::make_tuple(filename, line));
      lines_available_ul.unlock();
      lines_available_cv.notify_one();
    }
    files_available_ul.unlock(); // cosi' altri line consumer possono prenderlo
    files_available_cv.notify_one();
  }
  cout_stream.lock();
  std::cout << "Line producer (" << i << ") ha terminato la sua esecuzione" << std::endl;
  cout_stream.unlock();
  return;
}

void files_producer() {
  PROFILE_FUNCTION();
  // creazione della coda di fileJobs da cui devono prendere gli n producer
  for (const std::filesystem::directory_entry &it: std::filesystem::recursive_directory_iterator(directory_path)) {
    fileJobs.put(it.path());
    //cout_stream.lock();
    //std::cout << "FileJobs current size: " << fileJobs.size() << std::endl;
    //std::cout << "FileJobs real current size: " << fileJobs.jobs.size() << std::endl;
    //cout_stream.unlock();
    //producers.emplace_back(std::thread(producer, i, it.path()));
    /*    std::packaged_task<void ()> new_task(std::bind(producer,it.path()));
        fileJobs.put(std::move(new_task));*/
  }

  auto n_core = std::thread::hardware_concurrency();
  auto n_threads = (n_core - 1) / 2;
  std::vector<std::thread> producers;
  producers.reserve(n_threads); //3 threads producers
  std::vector<std::thread> consumers;
  consumers.reserve(n_threads); //3 threads consumers

  /*  for (auto i = 0; i < n_threads; i++) {
      producers.emplace_back(std::thread(lines_producer, i));
      consumers.emplace_back(std::thread(consumer, (i + 10)));
    }*/
  for (auto i = 0; i < 50; i++) {
    producers.emplace_back(std::thread(lines_producer, i));
  }
  for (auto i = 0; i < 20; i++) {
    consumers.emplace_back(std::thread(consumer, (i + 10)));
  }
  for (auto &a: producers)
    a.join();

  /*  directory_completed_m.lock();
    directory_completed = true;
    directory_completed_m.unlock();*/
  directory_completed.store(true);
  lines_available_cv.notify_all(); //necessario sbloccare tutti quelli rimasti nella wait per farli terminare!
  for (auto &a: consumers)
    a.join();
}
int main(int argc, char *argv[]) {
  Instrumentor::Instance().beginSession("Profile1");
  /*  if(argc!=3){
      exit(-1);
    }
    std::string directory_path = argv[1];
    std::string regex = argv[2];*/

  /*  auto n_core = std::thread::hardware_concurrency();
    auto n_threads = (n_core - 1) / 2;
    std::vector<std::thread> producers;
    producers.reserve(n_threads); //3 threads producers
    std::vector<std::thread> consumers;
    consumers.reserve(n_threads); //3 threads consumers

    for (auto i = 0; i < n_threads; i++) {
      producers.emplace_back(std::thread(producer, i));
      consumers.emplace_back(std::thread(consumer, (i + 10)));
    }
    // 3 thread producer attenderanno che la coda di fileJobs si riempia per partire: files_available condition
    // variable locked quando coda vuota, unlocked quando piena
    // e termineranno quando directory_completed diventa unlocked

    // 3 thread consumer attenderanno che la coda di lineJobs si riempia per partire: lines_available condition
    // variable locked quando coda vuota, unlocked quando piena
    // e termineranno quando directory_completed diventa unlocked
    */
  files_producer();




  /*  for (auto &a: consumers)
      a.join();*/
  Instrumentor::Instance().endSession();
  return 0;
}
