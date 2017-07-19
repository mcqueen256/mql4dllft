#include "catch.hpp"

#include <iostream>
#include <exception> // std::bad_function_call, std::runtime_error
#include <thread> // std::thread, std::this_thread::yield
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <functional> // std::function

class EventThreader {
public:
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

EventThreader::EventThreader() {
}
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
		EventThreader et;
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
		    et.allocation_mtx.lock();
			if (et.exception_from_the_event_thread != nullptr) {
				delete et.exception_from_the_event_thread;
				et.exception_from_the_event_thread = nullptr;
			}
			if (et.calling_lock != nullptr) {
				delete et.calling_lock;
				et.calling_lock = nullptr;
			}
			if (et.event_lock != nullptr) {
				delete et.event_lock;
				et.event_lock = nullptr;
			}
			et.allocation_mtx.unlock();
		};

		auto join = [&]() {
		    et.allocation_mtx.lock();
		    delete et.calling_lock; // remove lock on this thread, allow event to run
		    et.calling_lock = nullptr;
		    et.allocation_mtx.unlock();
		    if (et.event_lock != nullptr) {
		        et.event_waiter.notify_one();
		        std::this_thread::yield();
		    }
		    et.event_thread.join();
		    if (et.exception_from_the_event_thread != nullptr) {
		        /* an exception occured */
		        std::runtime_error e_copy(et.exception_from_the_event_thread->what());
		        et.allocation_mtx.lock();
		        delete et.exception_from_the_event_thread;
		        et.exception_from_the_event_thread = nullptr;
		        et.allocation_mtx.unlock();
		        throw e_copy;
		    }
		    deallocate();
		};

	    auto switchToCallingThread = [&]() {
		    if (!et.require_switch_from_event) {
		        throw std::runtime_error("switch to calling not matched with a switch to event");
		    }
		    et.require_switch_from_event = false;
		    /* switch to calling */
		    et.calling_waiter.notify_one();
		    std::this_thread::yield();
		    et.event_waiter.wait(*(et.event_lock));
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
		et.allocation_mtx.lock();
	    et.exception_from_the_event_thread = nullptr;
	    et.event_lock = nullptr;
	    et.calling_lock = nullptr;
	    et.calling_lock = new std::unique_lock<std::mutex>(et.mtx);
	    et.allocation_mtx.unlock();
	    et.exception_from_the_event_thread = nullptr;

	    et.event_cleanup = [](){}; // empty function
	    auto event = [&](){
	        /* mtx force switch to calling - blocked by the mutex */
	        et.allocation_mtx.lock();
	        et.event_lock = new std::unique_lock<std::mutex>(et.mtx);
	        et.allocation_mtx.unlock();

	        et.calling_waiter.notify_one();
	        et.event_waiter.wait(*(et.event_lock));
	        std::this_thread::yield();
	        try {
	            func([&](){switchToCallingThread();});
	            if (et.require_switch_from_event) { // the event has ended, but not ready to join
	                // rejoin the calling thread after dealing with this exception
	                throw std::runtime_error("switch to event not matched with a switch to calling");
	            }
	        } catch (const std::runtime_error &e) {
	            /* report the exception to the calling thread */
	            et.allocation_mtx.lock();
	            et.exception_from_the_event_thread = new std::runtime_error(e);
	            et.allocation_mtx.unlock();
	            et.calling_waiter.notify_one();
	            std::this_thread::yield();
	        }
	        et.allocation_mtx.lock();
	        delete et.event_lock;
	        et.event_lock = nullptr;
	        et.allocation_mtx.unlock();
	        et.event_cleanup();
	    };

	    et.event_thread = std::thread(event);
	    std::this_thread::yield();
	    et.calling_waiter.wait(*(et.calling_lock));
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

	} */
}
