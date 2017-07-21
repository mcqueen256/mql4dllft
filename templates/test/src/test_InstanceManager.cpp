#include <stdexcept>      // std::out_of_range

#include "catch.hpp"

#include "RobotReferenceType.hpp"
#include "InstanceManager.hpp"
#include "export.hpp"
#include "mql.hpp"

int stringToReference(std::string s) {
	int ref = createStringReference();
	for (auto c: s) {
		stringAddChar(ref, c);
	}
	return ref;
}

TEST_CASE( "Initialisation", "[InstanceManager]" ) {
	// settings
	std::string robot_name = "r1";
	//Init
	RobotReferenceType instance = 0;
	REQUIRE( instance == 0);
	int robot_name_ref = stringToReference(robot_name);
	instance = initialise(robot_name_ref);
	REQUIRE( instance != 0);
	REQUIRE( InstanceManager::stringAt(robot_name_ref) == false );
	REQUIRE_NOTHROW([&instance](){
		deinitialise(instance, static_cast<unsigned>(mql::REASON::PROGRAM));
	});
}

TEST_CASE( "de-initialisation", "[InstanceManager]" ) {
	// settings
	std::string robot_name = "r1";
	//Init
	int robot_name_ref = stringToReference(robot_name);
	SECTION("Expert Advisor terminated its operation by calling the ExpertRemove() function") {
		RobotReferenceType ref = initialise(robot_name_ref);
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::PROGRAM));
		});
	}

	SECTION("Double de-initialisation") {
		RobotReferenceType ref = initialise(robot_name_ref);
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::PROGRAM));
		});
		// No error is to be thrown
		REQUIRE_NOTHROW([&ref](){
			deinitialise(ref, static_cast<unsigned>(mql::REASON::PROGRAM));
		});
	}

}

TEST_CASE( "String handling", "[InstanceManager]" ) {
	SECTION("Close access test") {
		std::string s = "this is the string";
		int ref = InstanceManager::stringNewReference();
		REQUIRE( InstanceManager::stringAt(ref) == true );
		for (auto c : s) {
			InstanceManager::stringAddChar(ref, c);
		}
		std::string t = InstanceManager::stringGet(ref);
		REQUIRE( InstanceManager::stringAt(ref) == false );
		REQUIRE( t == s );
	}

	SECTION("As if mql") {
		std::string s = "this is the string";

		int ref = createStringReference();
		for (auto c : s) {
			stringAddChar(ref, c);
		}
		std::string t = InstanceManager::stringGet(ref);
		REQUIRE( t == s );
	}
}


