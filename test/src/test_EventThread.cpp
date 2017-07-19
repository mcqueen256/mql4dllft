#include "catch.hpp"

#include <iostream>
#include <exception> // std::bad_function_call, std::runtime_error
#include <thread> // std::thread, std::this_thread::yield
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

EventThreader::EventThreader(std::function<void (std::function<void (void)>)> func) {
	allocation_mtx.lock();
    calling_lock = new std::unique_lock<std::mutex>(mtx);
    allocation_mtx.unlock();
    event_cleanup = [](){}; // empty function
    event_function = func;
    auto event = [&](){
        /* mtx force switch to calling - blocked by the mutex */
        this->allocation_mtx.lock();
        this->event_lock = new std::unique_lock<std::mutex>(this->mtx);
        this->allocation_mtx.unlock();

        this->calling_waiter.notify_one();
        this->event_waiter.wait(*(this->event_lock));
        std::this_thread::yield();
        try {
            this->event_function([&](){this->switchToCallingThread();});
            if (this->require_switch_from_event) { // the event has ended, but not ready to join
                // rejoin the calling thread after dealing with this exception
                throw std::runtime_error("switch to event not matched with a switch to calling");
            }
        } catch (const std::runtime_error &e) {
            /* report the exception to the calling thread */
            this->allocation_mtx.lock();
            this->exception_from_the_event_thread = new std::runtime_error(e);
            this->allocation_mtx.unlock();
            this->calling_waiter.notify_one();
            std::this_thread::yield();
        }
        this->allocation_mtx.lock();
        delete this->event_lock;
        this->event_lock = nullptr;
        this->allocation_mtx.unlock();
        this->event_cleanup();
    };

    event_thread = std::thread(event);
    std::this_thread::yield();
    calling_waiter.wait(*calling_lock);
    std::this_thread::yield();
}

EventThreader::~EventThreader() {
	deallocate();
}

void EventThreader::deallocate() {
	allocation_mtx.lock();
	if (exception_from_the_event_thread != nullptr) {
		delete exception_from_the_event_thread;
		exception_from_the_event_thread = nullptr;
	}
	if (calling_lock != nullptr) {
		delete calling_lock;
		calling_lock = nullptr;
	}
	if (event_lock != nullptr) {
		delete event_lock;
		event_lock = nullptr;
	}
	allocation_mtx.unlock();
}

void EventThreader::switchToCallingThread() {
	if (!require_switch_from_event) {
        throw std::runtime_error("switch to calling not matched with a switch to event");
    }
    require_switch_from_event = false;
    /* switch to calling */
    calling_waiter.notify_one();
    std::this_thread::yield();
    event_waiter.wait(*(event_lock));
    std::this_thread::yield();
    /* back from calling */
}

void EventThreader::switchToEventThread() {
	if (require_switch_from_event) {
        throw std::runtime_error("switch to event not matched with a switch to calling");
    }
    require_switch_from_event = true;
    /* switch to event */
    event_waiter.notify_one();
    std::this_thread::yield();
    calling_waiter.wait(*calling_lock);
    std::this_thread::yield();
    /* back from event */
    if (require_switch_from_event) {
        /* this exception is thrown if switchToCallingThread() was not used, which means the thread ended */
        join();
    }
}

void EventThreader::join() {
	allocation_mtx.lock();
    delete calling_lock; // remove lock on this thread, allow event to run
    calling_lock = nullptr;
    allocation_mtx.unlock();
    if (event_lock != nullptr) {
        event_waiter.notify_one();
        std::this_thread::yield();
    }
    event_thread.join();
    if (exception_from_the_event_thread != nullptr) {
        /* an exception occured */
        std::runtime_error e_copy(exception_from_the_event_thread->what());
        allocation_mtx.lock();
        delete exception_from_the_event_thread;
        exception_from_the_event_thread = nullptr;
        allocation_mtx.unlock();
        throw e_copy;
    }
    deallocate();
}

void EventThreader::setEventCleanup(std::function<void(void)> cleanup) {
    event_cleanup = cleanup;
}

TEST_CASE( "EventThreader", "[EventThreader]" ) {
	std::stringstream ss;

	SECTION("Finding the error") {

		/* Example of most basic use */
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			for(int i = 0; i < 100; i++) { ss << "*"; }
			switchToMainThread();
			for(int i = 0; i < 50; i++) { ss << "*"; }
			switchToMainThread();
		};

		EventThreader et(f);
		et.switchToEventThread();
		for(int i = 0; i < 75; i++) { ss << "$"; }
		et.switchToEventThread();
		for(int i = 0; i < 25; i++) { ss << "$"; }
		et.join();

		/* Generate what the result should look like */
		std::string requirement;
		for(int i = 0; i < 100; i++) { requirement += "*"; }
		for(int i = 0; i < 75; i++) { requirement += "$"; }
		for(int i = 0; i < 50; i++) { requirement += "*"; }
		for(int i = 0; i < 25; i++) { requirement += "$"; }
		REQUIRE( requirement == ss.str());
	}

	SECTION("Abitrary use") {
		// Example of most basic use
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			for(int i = 0; i < 100; i++) { ss << "*"; }
			switchToMainThread();
			for(int i = 0; i < 50; i++) { ss << "*"; }
			switchToMainThread();
		};
		EventThreader et(f);
		et.switchToEventThread();
		for(int i = 0; i < 75; i++) { ss << "$"; }
		et.switchToEventThread();
		for(int i = 0; i < 25; i++) { ss << "$"; }
		et.join();

		// Generate what the result should look like 
		std::string requirement;
		for(int i = 0; i < 100; i++) { requirement += "*"; }
		for(int i = 0; i < 75; i++) { requirement += "$"; }
		for(int i = 0; i < 50; i++) { requirement += "*"; }
		for(int i = 0; i < 25; i++) { requirement += "$"; }
		REQUIRE( requirement == ss.str());

	}
}
