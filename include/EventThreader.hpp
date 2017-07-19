#ifndef EVENTTHREADER_HPP
#define EVENTTHREADER_HPP

#include <exception> // std::runtime_error
#include <thread> // std::thread
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <functional> // std::function

class EventThreader {
public:
    std::condition_variable event_waiter;
    std::condition_variable calling_waiter;
    std::unique_lock<std::mutex>* event_lock = nullptr;
    std::unique_lock<std::mutex>* calling_lock = nullptr;
    std::mutex mtx;
    std::mutex allocation_mtx;
    std::thread event_thread;
    void switchToCallingThread();
    bool require_switch_from_event = false;
    std::function<void(void)> event_cleanup;
    std::runtime_error* exception_from_the_event_thread = nullptr;
    std::function<void (std::function<void (void)>)> event_function;
    void deallocate();
public:
    EventThreader(std::function<void (std::function<void (void)>)> func);
    ~EventThreader();
    void switchToEventThread();
    void join();
    void setEventCleanup(std::function<void(void)>);
};

#endif