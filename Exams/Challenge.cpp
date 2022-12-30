// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 15/01/22.
//

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 15/01/22.
//

#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

class Challenge {
  //Gioco online tra due giocatori, gestione di più thread.
 private:
  bool accepted;
  int res1;
  int res2;
  bool acceptedcalled;
  bool game_ended;
  std::mutex game_started_m;
  std::condition_variable game_started_cv;
 public:
  Challenge() : accepted(false), acceptedcalled(false), game_ended(false), res1(-1), res2(-1) {
  }

  //Tutti e 3 possono essere chiamati una sola volta, la violazione dei vincoli genera eccezione.
  void accept(bool response) {
    if (acceptedcalled)
      throw std::runtime_error("Accepted already called!");
    //Indica il responso dello sfidato alla richiesta di giocare
    //`accept()` deve precedere `result1` e` result2`
    std::unique_lock<std::mutex> ul(game_started_m);
    if (!response) {
      game_ended = true;
    } else
      accepted = true;
    acceptedcalled = true;
    game_started_cv.notify_all();
  }
  void result1(int score) {

    std::unique_lock<std::mutex> ul(game_started_m);
    game_started_cv.wait(ul, [this]() { return accepted; });
    //Indica lo score dello sfidante
    if (!acceptedcalled)
      throw std::runtime_error("Result1 called before accept!");
    if (res1 != -1)
      throw std::runtime_error("Result1 already called!");
    res1 = score;
    game_started_cv.notify_all();

    if (res2 != -1) {
      game_ended = true;
      game_started_cv.notify_all();
    }

  }

  void result2(int score) {

    std::unique_lock<std::mutex> ul(game_started_m);
    game_started_cv.wait(ul, [this]() { return accepted; });
    if (!acceptedcalled)
      throw std::runtime_error("Result2 called before accept!");
    if (res2 != -1)
      throw std::runtime_error("Result2 already called!");
    //Indica lo score dello sfidato
    res2 = score;
    game_started_cv.notify_all();

    if (res1 != -1) {
      game_ended = true;
      game_started_cv.notify_all();
    }

  }
  int winner() {
    //winner()` si mette in attesa senza consumare CPU se il gioco è in corso o non è ancora cominciato. -> condition_variable
    std::unique_lock<std::mutex> ul(game_started_m);
    game_started_cv.wait(ul, [this]() { return game_ended; });
    if (!accepted)
      return -1;
    int res = res1 - res2;
    if (res == 0)
      return 0;
    if (res > 0)
      return 1;
    if (res < 0)
      return 2;
    return -2;
  }
};

int main() {
  Challenge ch;

  std::thread threads[2];

  threads[0] = std::thread([&ch]() {
    /*    try {
          ch.accept(true);
        } catch (const std::exception &e) {
          std::cerr << e.what() << std::endl;
          exit(-1);
        }*/
    std::cout << "Ha vinto... " << ch.winner() << std::endl;
    std::cout << "Ha vinto... " << ch.winner() << std::endl;

  });
  threads[1] = std::thread([&ch]() {
    //std::cout << "Ha vinto... " << ch.winner() << std::endl;

    try {
      ch.accept(true);
    } catch (const std::exception &e) {
      std::cerr << e.what() << std::endl;
      exit(-1);
    }
    ch.result2(2);
    ch.result1(4);

    std::cout << "Ha vinto... " << ch.winner() << std::endl;
  });

  for (auto &thread: threads)
    thread.join();
}