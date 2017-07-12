#ifndef EVENTTHREADER_HPP
#define EVENTTHREADER_HPP

#include <iostream>
#include <exception> // std::bad_function_call, std::runtime_error
#include <thread> // std::thread, std::this_thread::yield
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <functional> // std::function

/**
 * The EventThreader class runs two threads simutaniously but not at the same
 * time. The switching between threads is controlled by the EventThreader
 * class. The two threads are the current thread (named the calling thread)
 * and the second thread is spawned from a given function (event thread). The
 * event thread is spawned and paused in the construction of the EventThreader
 * class.
 *
 * Operation
 * Only the calling thread will run until it requests that event thread is run.
 * While one thread is running, it is guaranteed that the other is paused. On
 * EventThreader construction, the event thead is paused. The event thread is
 * invoked (and the calling thread paused) either with calls
 * EventThreader::join() or EventThreader::switchToEventThread(). From the
 * event thread, the calling thread can be invoked by a call to the event
 * threads function parameter.
 *
 * The EventThreader takes one parameter on construction to be a functional
 * object f of type void(void(void)). f will be the function run as the event
 * thread. f's parameter is the function that invokes the event thread to
 * pause, allowing the calling thread to continue.
 *
 * Every call to EventThreader::switchToEventThread() from the calling thread
 * must be followed by a call EventThreader::switchToCallingThread() via
 * calling the event theads functional parameter from the event thread.
 */
class EventThreader {
private:
    std::condition_variable event_waiter, calling_waiter;
    std::unique_lock<std::mutex>* event_lock, * calling_lock;
    std::mutex mtx, allocation_mtx;
    std::thread event_thread;
    void switchToCallingThread();
    bool require_switch_from_event = false;
    std::function<void(void)> event_cleanup;
    std::runtime_error* exception_from_the_event_thread;
    void deallocate();
public:
    EventThreader(std::function<void (std::function<void (void)>)> func);
    ~EventThreader();
    void switchToEventThread();
    void join();
    void setEventCleanup(std::function<void(void)>);
};



#endif
