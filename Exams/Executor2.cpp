// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//
// Created by bexul on 15/01/22.
//

#include <iostream>
#include <future>
#include <functional>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

class Context{
    std::string s = "default";
public:
    Context(std::string string):s(string){}
    void print(){
        std::cout << s << std::endl;
    }
};


void func(Context* c){
    c->print();
}

class Executor {
    std::queue<std::future<void>> queue;
    Context* context;
    std::mutex m;
    std::condition_variable cv;
    bool sd = false;
    std::thread t;
public:
    Executor(Context *ctx){
        context = ctx;
        t = std::move(std::thread([this](){
            terminate_threads();
        }));
    }
    ~Executor(){
        t.join();
    }
    std::future<void> submit(std::function<void(Context*)> f){
        std::lock_guard lg(m);
        if(sd)
            throw std::logic_error("Errore");

        std::packaged_task<void(Context*)> pt(f);
        std::future<void> future = pt.get_future();
        pt(context);
        queue.push(std::move(future));
        cv.notify_one();

        return future;
    }
    void shutdown(){
        std::lock_guard lg(m);
        sd = true;
    }

    void terminate_threads(){
        std::cout << "entered terminate threads" << std::endl;
        while(!sd){
            std::unique_lock ul(m);
            cv.wait(ul,[this](){return !queue.empty();});
            std::cout << " get one future" << std::endl;
            std::future<void> future = std::move(queue.front());
            queue.pop();

            if (std::future_status::ready != future.wait_for(std::chrono::seconds(0))) {
                std::cout << "Waiting for results..." << std::endl;
                future.wait();

            }

        }
    }
};


int main() {
    Context* c= new Context("ciao");
    Executor exec(c);

    exec.submit([](Context* c){
        std::this_thread::sleep_for(std::chrono::seconds(5));
        c->print();
    });
    std::this_thread::sleep_for(std::chrono::seconds(3));
    exec.submit([](Context* c){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        c->print();
    });

    exec.submit([](Context* c){
        std::this_thread::sleep_for(std::chrono::seconds(10));
        c->print();
    });

    exec.submit(func);

    exec.shutdown();

    //exec.submit(func);

    return 0;
}
