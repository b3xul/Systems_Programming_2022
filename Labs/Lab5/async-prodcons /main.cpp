// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <vector>
#include <future>
#include <numeric>

std::mutex mtx;

template<typename T>
int consumer(T begin, T end) {
  typename T::difference_type len = end - begin;
  if (len < 1000) {
    return std::accumulate(begin, end, 0);
  }
  T mid = begin + len / 2;
  auto res = std::async(std::launch::async, consumer<T>, mid, end);
  int sum = consumer(begin, mid);
  return sum + res.get();
}

void producer(int i) {
  std::vector<int> v(10000, 1);
  int res = consumer(v.begin(), v.end());

  mtx.lock();
  std::cout << "Producer: " << i << " result: " << res << std::endl;
  mtx.unlock();

  //cout stream non e' thread safe!
  // Producer: Producer: 21 result:  result: 1000010000
  //
  //Producer: 6 result: 10000
}

int main() {

  std::vector<std::thread> producers;
  producers.reserve(10);
  for (int i = 0; i < 10; i++) {
    producers.emplace_back(std::thread(producer, i));
    producers.emplace_back(std::thread(producer, i));
  }

  for (auto &a: producers)
    a.join();

  return 0;
}
