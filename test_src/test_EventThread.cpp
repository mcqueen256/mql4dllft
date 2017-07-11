#include "catch.hpp"

#include <iostream> // std::cout, std::endl
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
    std::mutex mtx;
    std::thread event_thread;
    void switchToCallingThread();
    bool request_end_of_event = false;
    bool waiting_for_event = false;
    bool event_ended = false;
    std::function<void(void)> event_cleanup;
    std::runtime_error* exception_from_the_event_thread;
public:
    EventThreader(std::function<void (std::function<void (void)>)> func);
    ~EventThreader();
    void switchToEventThread();
    void join();
    void setEventCleanup(std::function<void(void)>);
};

EventThreader::EventThreader(std::function<void (std::function<void (void)>)> func) {
    exception_from_the_event_thread = nullptr;
    calling_lock = new std::unique_lock<std::mutex>(mtx);
    event_cleanup = [](){}; // empty function
    auto event = [&](){
        /* mtx force switch to calling - blocked by the mutex */
        event_lock = new std::unique_lock<std::mutex>(mtx);
        calling_waiter.notify_one();
        event_waiter.wait(*event_lock);
        std::this_thread::yield();
        try {
            func([&](){switchToCallingThread();});
            if (waiting_for_event) { // the event has ended, but not ready to join
                // rejoin the calling thread after dealing with this exception
                calling_waiter.notify_one();
                throw std::runtime_error("calling thread is not ready to join");
            }
        } catch (const std::runtime_error &e) {
            /* report the exception to the calling thread */
            exception_from_the_event_thread = new std::runtime_error(e);
        }
        delete event_lock;
        event_cleanup();
        event_ended = true;
    };
    
    event_thread = std::thread(event);
    std::this_thread::yield();
    calling_waiter.wait(*calling_lock);
    std::this_thread::yield();
}

EventThreader::~EventThreader() {}

void EventThreader::switchToCallingThread() {
    if (request_end_of_event) {
        std::runtime_error err("calling thread requested join, cannot call switchToCallingThread()");
        throw err;
    }
    /* switch to calling */
    calling_waiter.notify_one();
    std::this_thread::yield();
    event_waiter.wait(*event_lock);
    std::this_thread::yield();
    /* back from calling */
}

void EventThreader::switchToEventThread() {
    if (event_ended) {
        throw std::runtime_error("event thread ended, cannot switch to it");
    }
    waiting_for_event = true;
    /* switch to event */
    event_waiter.notify_one();
    std::this_thread::yield();
    calling_waiter.wait(*calling_lock);
    std::this_thread::yield();
    /* back from event */
    waiting_for_event = false;
}

void EventThreader::join() {
    request_end_of_event = true;
    delete calling_lock; // remove lock on this thread, allow event to run
    event_waiter.notify_one();
    std::this_thread::yield();
    event_thread.join();
    if (exception_from_the_event_thread != nullptr) {
        /* an exception occured */
        std::runtime_error e_copy(exception_from_the_event_thread->what());
        delete exception_from_the_event_thread;
        throw e_copy;
    }
}

void EventThreader::setEventCleanup(std::function<void(void)> cleanup) {
    event_cleanup = cleanup;
}

TEST_CASE( "EventThreader", "[EventThreader]" ) {
	std::stringstream ss;

	SECTION("Abitrary use") {
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

	SECTION("Empty event - no switch, most basic use") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){};
		EventThreader et(f);
		et.join();
		REQUIRE( ss.str() == "" );
	}

	SECTION("Empty event - no switch, main prints a thing") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){};
		EventThreader et(f);
		ss << "here";
		et.join();
		REQUIRE( ss.str() == "here" );
	}

	SECTION("event is run without switch") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		et.join();
		REQUIRE( ss.str() == "f" );
	}

	SECTION("event is run at join (not asked to run before)") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		ss << "p";
		et.join();
		REQUIRE( ss.str() == "pf" );
	}

	SECTION("one switch to event without a switch to main") {
		auto f = [&ss](){
			auto f = [&ss](std::function<void(void)> switchToMainThread){
				ss << "f";
			};
			EventThreader et(f);
			et.switchToEventThread();
			et.join();
		};
		REQUIRE_THROWS_AS(f(), std::runtime_error);
		REQUIRE( ss.str() == "f" );
		REQUIRE_THROWS_WITH(f(), "calling thread is not ready to join");
		REQUIRE( ss.str() == "ff" );
	}

	SECTION("two switches to event with no switches to main") {
		auto f = [&ss](){
			auto f = [&ss](std::function<void(void)> switchToMainThread){
				ss << "f";
			};
			EventThreader et(f);
			et.switchToEventThread();
			et.switchToEventThread();
			et.join();
		};
		REQUIRE_THROWS_AS(f(), std::runtime_error);
		REQUIRE( ss.str() == "f" );
		REQUIRE_THROWS_WITH(f(), "event thread ended, cannot switch to it");
		REQUIRE( ss.str() == "ff" );
	}

	SECTION("print order") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << 1;
			switchToMainThread();
			ss << 3;
			switchToMainThread();
			ss << 5;
			switchToMainThread();
		};
		EventThreader et(f);
		ss << 0;
		et.switchToEventThread();
		ss << 2;
		et.switchToEventThread();
		ss << 4;
		et.switchToEventThread();
		ss << 6;
		et.join();
		REQUIRE( ss.str() == "0123456" );
	}

	SECTION("no event switch to main") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "1";
			ss << "2";
		};
		EventThreader et(f);
		et.join();
		REQUIRE( ss.str() == "12" );
	}

	SECTION("one event switch to main") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "1";
			switchToMainThread();
			ss << "2";
		};
		EventThreader et(f);
		et.switchToEventThread();
		et.join();
		REQUIRE( ss.str() == "1" );
	}

	SECTION("premiture join with exception message") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			switchToMainThread();
			switchToMainThread();
		};
		EventThreader et(f);
		et.switchToEventThread();
		REQUIRE_THROWS_WITH([&et](){
			et.join();
		}, "EventThreader attempted join but event thread is still running");
	}

	SECTION("EventThreader when event still running") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			switchToMainThread();
			switchToMainThread();
		};
		EventThreader* et = new EventThreader(f);
		et->switchToEventThread();
		REQUIRE_THROWS_WITH([&et](){
			delete et;
		}(), "EventThreader attempted deconstructor but event thread is still running");
	}
}
