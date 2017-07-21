#include <stdexcept>      // std::out_of_range

#include "catch.hpp"

#include "RobotReferenceType.hpp"
#include "export.hpp"
#include "mql.hpp"

TEST_CASE( "Initialisation", "[InstanceManager]" ) {
	RobotReferenceType instance = 0;
	REQUIRE( instance == 0);
	instance = initialise();
	REQUIRE( instance != 0);
	REQUIRE_NOTHROW([&instance](){
		deinitialise(instance, static_cast<unsigned>(mql::REASON::PROGRAM));
	});
}

TEST_CASE( "de-initialisation", "[InstanceManager]" ) {

	SECTION("Expert Advisor terminated its operation by calling the ExpertRemove() function") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::PROGRAM));
		});
	}

	SECTION("Program has been deleted from the chart") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::REMOVE));
		});
	}

	SECTION("Program has been recompiled") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::RECOMPILE));
		});
	}

	SECTION("Symbol or chart period has been changed") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::CHARTCHANGE));
		});
	}

	SECTION("Chart has been closed") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::CHARTCLOSE));
		});
	}

	SECTION("Input parameters have been changed by a user") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::PARAMETERS));
		});
	}

	SECTION("Another account has been activated or reconnection to the trade server has occurred due to changes in the account settings") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::ACCOUNT));
		});
	}

	SECTION("A new template has been applied") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::TEMPLATE));
		});
	}

	SECTION("This value means that OnInit() handler has returned a nonzero value") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::INITFAILED));
		});
	}

	SECTION("Terminal has been closed") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::CLOSE));
		});
	}

	SECTION("Double de-initialisation") {
		RobotReferenceType ref = initialise();
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::PROGRAM));
		});
		// No error is to be thrown
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::PROGRAM));
		});
	}

}