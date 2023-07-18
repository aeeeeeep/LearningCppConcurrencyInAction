#include <iostream>
#include <thread>

void do_something(int& i) {
    i*=2;
    i/=2;
}

struct func {
    int& i;
    func(int& _i) : i(_i) {}
    void operator() () {
        for(unsigned j=0; j<10000000; j++) {
            do_something(i);
        }
    }
};

void f_oops() {
    int local_state = 0;
    func i_func(local_state);
    std::thread t(i_func);
    t.detach();
}

void f_try() {
    int local_state = 42;
    func i_func(local_state);
    std::thread t(i_func);
    try {
        std::cout<<"-1"<<std::endl;
        throw std::runtime_error("Exception in current thread");
    } catch(...) {
        std::cout<<"0"<<std::endl;
        t.join();
        std::cout<<"1"<<std::endl;
        throw;
    }
    std::cout<<"2"<<std::endl;
    t.join();
    std::cout<<"3"<<std::endl;
}

class thread_guard {
    std::thread& t;
public:
    explicit thread_guard(std::thread& _t):t(_t) {}
    ~thread_guard() {
        if(t.joinable()) {
            std::cout<<"4"<<std::endl;
            t.join();
            std::cout<<"5"<<std::endl;
        }
    }
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;
};

void f_guard() {
    int local_state=42;
    func i_func(local_state);
    std::thread t(i_func);
    thread_guard g(t);
}

int main() {
    f_oops();
    // f_try();
    f_guard();
    return 0;
}
