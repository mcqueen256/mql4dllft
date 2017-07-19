#include "catch.hpp"

#include <iostream>
#include <exception> // std::bad_function_call, std::runtime_error
#include <thread> // std::thread, std::this_thread::yield
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <functional> // std::function
#define ee.

class EventThreader {
public:
    std::condition_variable event_waiter;
    std::condition_variable calling_waiter;
    std::unique_lock<std::mutex>* event_lock = nullptr;
    std::unique_lock<std::mutex>* calling_lock = nullptr;
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

EventThreader::EventThreader(std::function<void (std::function<void (void)>)> func) {
}

EventThreader::~EventThreader() {
}

void EventThreader::deallocate() {
}

void EventThreader::switchToCallingThread() {
}

void EventThreader::switchToEventThread() {
}

void EventThreader::join() {
}

void EventThreader::setEventCleanup(std::function<void(void)> cleanup) {
    event_cleanup = cleanup;
}

TEST_CASE( "EventThreader", "[EventThreader]" ) {
	std::stringstream ss;

	SECTION("Finding the error") {
		EventThreader et([](std::function<void(void)> f){});
		// class variables
		std::condition_variable event_waiter, calling_waiter;
	    std::unique_lock<std::mutex>* event_lock, * calling_lock;
	    std::mutex mtx, allocation_mtx;
	    std::thread event_thread;
	    bool require_switch_from_event = false;
	    std::function<void(void)> event_cleanup;
	    std::runtime_error* exception_from_the_event_thread;

	    // class functions

		auto deallocate = [&]() {
		    ee.allocation_mtx.lock();
			if (ee.exception_from_the_event_thread != nullptr) {
				delete ee.exception_from_the_event_thread;
				ee.exception_from_the_event_thread = nullptr;
			}
			if (ee.calling_lock != nullptr) {
				delete ee.calling_lock;
				ee.calling_lock = nullptr;
			}
			if (ee.event_lock != nullptr) {
				delete ee.event_lock;
				ee.event_lock = nullptr;
			}
			ee.allocation_mtx.unlock();
		};

		auto join = [&]() {
		    ee.allocation_mtx.lock();
		    delete ee.calling_lock; // remove lock on this thread, allow event to run
		    ee.calling_lock = nullptr;
		    ee.allocation_mtx.unlock();
		    if (ee.event_lock != nullptr) {
		        ee.event_waiter.notify_one();
		        std::this_thread::yield();
		    }
		    ee.event_thread.join();
		    if (ee.exception_from_the_event_thread != nullptr) {
		        /* an exception occured */
		        std::runtime_error e_copy(ee.exception_from_the_event_thread->what());
		        ee.allocation_mtx.lock();
		        delete ee.exception_from_the_event_thread;
		        ee.exception_from_the_event_thread = nullptr;
		        ee.allocation_mtx.unlock();
		        throw e_copy;
		    }
		    deallocate();
		};

	    auto switchToCallingThread = [&]() {
		    if (!ee.require_switch_from_event) {
		        throw std::runtime_error("switch to calling not matched with a switch to event");
		    }
		    ee.require_switch_from_event = false;
		    /* switch to calling */
		    ee.calling_waiter.notify_one();
		    std::this_thread::yield();
		    ee.event_waiter.wait(*(ee.event_lock));
		    std::this_thread::yield();
		    /* back from calling */
		};

		auto switchToEventThread= [&]() {
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
		};

		/* Example of most basic use */
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			for(int i = 0; i < 100; i++) { ss << "*"; }
			switchToMainThread();
			for(int i = 0; i < 50; i++) { ss << "*"; }
			switchToMainThread();
		};
		std::function<void (std::function<void (void)>)> func = f;

		// Start construction
		ee.allocation_mtx.lock();
	    ee.exception_from_the_event_thread = nullptr;
	    ee.event_lock = nullptr;
	    ee.calling_lock = nullptr;
	    ee.calling_lock = new std::unique_lock<std::mutex>(ee.mtx);
	    ee.allocation_mtx.unlock();
	    ee.exception_from_the_event_thread = nullptr;

	    ee.event_cleanup = [](){}; // empty function
	    auto event = [&](){
	        /* mtx force switch to calling - blocked by the mutex */
	        ee.allocation_mtx.lock();
	        ee.event_lock = new std::unique_lock<std::mutex>(ee.mtx);
	        ee.allocation_mtx.unlock();

	        ee.calling_waiter.notify_one();
	        ee.event_waiter.wait(*(ee.event_lock));
	        std::this_thread::yield();
	        try {
	            func([&](){switchToCallingThread();});
	            if (ee.require_switch_from_event) { // the event has ended, but not ready to join
	                // rejoin the calling thread after dealing with this exception
	                throw std::runtime_error("switch to event not matched with a switch to calling");
	            }
	        } catch (const std::runtime_error &e) {
	            /* report the exception to the calling thread */
	            ee.allocation_mtx.lock();
	            ee.exception_from_the_event_thread = new std::runtime_error(e);
	            ee.allocation_mtx.unlock();
	            ee.calling_waiter.notify_one();
	            std::this_thread::yield();
	        }
	        ee.allocation_mtx.lock();
	        delete ee.event_lock;
	        ee.event_lock = nullptr;
	        ee.allocation_mtx.unlock();
	        ee.event_cleanup();
	    };

	    ee.event_thread = std::thread(event);
	    std::this_thread::yield();
	    ee.calling_waiter.wait(*(ee.calling_lock));
	    std::this_thread::yield();
		// End constuction
		//EventThreader et(f);
		switchToEventThread();
		for(int i = 0; i < 75; i++) { ss << "$"; }
		switchToEventThread();
		for(int i = 0; i < 25; i++) { ss << "$"; }
		join();

		/* Generate what the result should look like */
		std::string requirement;
		for(int i = 0; i < 100; i++) { requirement += "*"; }
		for(int i = 0; i < 75; i++) { requirement += "$"; }
		for(int i = 0; i < 50; i++) { requirement += "*"; }
		for(int i = 0; i < 25; i++) { requirement += "$"; }
		REQUIRE( requirement == ss.str());

		deallocate();

	}

	/* SECTION("Abitrary use") {
		// Example of most basic use
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			for(int i = 0; i < 100; i++) { ss << "*"; }
			switchToMainThread();
			for(int i = 0; i < 50; i++) { ss << "*"; }
			switchToMainThread();
		};
		EventThreader et(f);
		ee.switchToEventThread();
		for(int i = 0; i < 75; i++) { ss << "$"; }
		ee.switchToEventThread();
		for(int i = 0; i < 25; i++) { ss << "$"; }
		ee.join();

		// Generate what the result should look like 
		std::string requirement;
		for(int i = 0; i < 100; i++) { requirement += "*"; }
		for(int i = 0; i < 75; i++) { requirement += "$"; }
		for(int i = 0; i < 50; i++) { requirement += "*"; }
		for(int i = 0; i < 25; i++) { requirement += "$"; }
		REQUIRE( requirement == ss.str());

	} */
}
