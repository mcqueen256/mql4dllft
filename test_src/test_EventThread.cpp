#include "catch.hpp"

#include <thread> // std::thread
#include <sstream> // std::stringstream
#include <functional> // std::function
#include <mutex>
#include <exception>
#include <thread> // std::thread, std::this_thread::yield
#include <mutex> // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <functional> // std::function

class EventThreader {
private:
    std::condition_variable event_waiter, calling_waiter;
    std::unique_lock<std::mutex>* event_lock, * calling_lock;
    std::mutex mtx;
    std::thread event_thread;
    void switchToCallingThread();
public:
    EventThreader(std::function<void (std::function<void (void)>)> func);
    ~EventThreader();
    void switchToEventThread();
    void join();
};

EventThreader::EventThreader(std::function<void (std::function<void (void)>)> func) {
    calling_lock = new std::unique_lock<std::mutex>(mtx);
    auto event = [&](){
        /* mtx force switch to calling - blocked by the mutex */
        event_lock = new std::unique_lock<std::mutex>(mtx);
        calling_waiter.notify_one();
        event_waiter.wait(*event_lock);
        std::this_thread::yield();
        func([&](){switchToCallingThread();});
        /* end - join to calling */
        delete event_lock;
    };
    
    event_thread = std::thread(event);
    std::this_thread::yield();
    calling_waiter.wait(*calling_lock);
    std::this_thread::yield();
}

EventThreader::~EventThreader() {}

void EventThreader::switchToCallingThread() {
    /* switch to calling */
    calling_waiter.notify_one();
    std::this_thread::yield();
    event_waiter.wait(*event_lock);
    std::this_thread::yield();
    /* back from calling */
}

void EventThreader::switchToEventThread() {
    /* switch to event */
    event_waiter.notify_one();
    std::this_thread::yield();
    calling_waiter.wait(*calling_lock);
    std::this_thread::yield();
    /* back from event */
}

void EventThreader::join() {
    delete calling_lock; // remove lock on this thread, allow event to run
    event_waiter.notify_one();
    std::this_thread::yield();
    event_thread.join();
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

	SECTION("no switch to event") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		et.join();
		REQUIRE( ss.str() == "" );
	}

	SECTION("one switch to event") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		et.switchToEventThread();
		et.join();
		REQUIRE( ss.str() == "f" );
	}

	SECTION("two switchs to event") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		et.switchToEventThread();
		et.switchToEventThread(); // will do nothing
		et.join();
		REQUIRE( ss.str() == "f" );
	}

	SECTION("print order") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << 1;
			switchToMainThread();
			ss << 3;
			switchToMainThread();
			ss << 5;
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
		et.switchToEventThread();
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
		}, "EventThreader attempted deconstructor but event thread is still running");
	}
}
