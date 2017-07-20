#include "catch.hpp"

#include "RobotReferenceType.hpp"

TEST_CASE( "Initialisation", "[InstanceManager]" ) {
	RobotReferenceType instance = 0;
	REQUIRE( instance == 0);
	

}