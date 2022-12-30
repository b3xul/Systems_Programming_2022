#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <queue>
#include <condition_variable>

template<typename T>
class Processor{
private:
    std::thread t;
    std::queue<T> coda;
    bool closed;
    std::mutex m;
    std::condition_variable cv;
    T value;
public:
    Processor(std::function<void(T)> f): closed(false){
        std::thread t1([&](){
            while(true) {
                std::unique_lock<std::mutex> ul(m);
                cv.wait(ul, [this](){return !coda.empty() || closed;});
                if(closed && coda.empty())
                    break;
                value = std::move(coda.front());
                coda.pop_front();
                ul.unlock();
                f(value);
                ul.lock();
            }
        });
        t = std::move(t1);
    }

    ~Processor(){
        closed = true;
        cv.notify_all();
        if(t.joinable())
            t.join();
    }

    void send(T t){
        std::lock_guard<std::mutex> lg(m);
        if(closed) throw std::runtime_error("Closed chiamata");
        coda.push(t);
        cv.notify_one()
    }

    void close(){
        std::unique_lock<std::mutex> ul(m);
        closed = true;
        cv.notify_one();
        ul.unlock();
        if(t.joinable()){
            t.join();
        }
    }
};




int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
