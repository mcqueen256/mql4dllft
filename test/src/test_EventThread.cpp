#include "catch.hpp"
#include "EventThreader.hpp"

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
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		REQUIRE_THROWS_AS(et.switchToEventThread(), std::runtime_error);
	}

	SECTION("one switch to event without a switch to main - text") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		REQUIRE_THROWS_WITH(et.switchToEventThread(), "switch to event not matched with a switch to calling");
	}

	SECTION("two switches to event with no switches to main") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		REQUIRE_THROWS_AS(et.switchToEventThread(), std::runtime_error);
		REQUIRE_THROWS_AS(et.switchToEventThread(), std::runtime_error);
	}

	SECTION("two switches to event with no switches to main - text") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			ss << "f";
		};
		EventThreader et(f);
		REQUIRE_THROWS_WITH(et.switchToEventThread(), "switch to event not matched with a switch to calling");
		REQUIRE_THROWS_WITH(et.switchToEventThread(), "switch to event not matched with a switch to calling");
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
		REQUIRE( ss.str() == "12" );
	}

	SECTION("premiture join with exception message") {
		auto f = [&ss](std::function<void(void)> switchToMainThread){
			switchToMainThread();
			switchToMainThread();
		};
		EventThreader et(f);
		et.switchToEventThread();
		REQUIRE_THROWS_WITH(et.join(), "switch to calling not matched with a switch to event");
	}
}
