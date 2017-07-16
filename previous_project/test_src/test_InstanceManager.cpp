#include "catch.hpp"
#include "export.h"

TEST_CASE( "InstanceManager as if MQL environment", "[InstanceManager]" ) {
	RobotReferenceType robotInstance = initialise(5);
	deinitialise(robotInstance, 0);
}