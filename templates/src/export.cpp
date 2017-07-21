#include "export.hpp"
#include "RobotReferenceType.hpp"

extern "C" {
	/*
	 * Note: MQL calls are not available until after this function returns.
	 */
	MT4_API RobotReferenceType initialise(int name_uid) {
		BaseRobot* robot = new BaseRobot();
		InstanceManager::push(robot);
		return reinterpret_cast<RobotReferenceType>(robot);
	}

	MT4_API void deinitialise(RobotReferenceType instance, const int reason) {
		/* Dealocate the robot memory. */
		delete InstanceManager::at(instance);
		/* Remove robot from existing instances. */
		InstanceManager::erase(instance);
	}

    MT4_API int createStringReference() {
    	return InstanceManager::stringNewReference();
    }

    MT4_API void stringAddChar(int uid, char c) {
    	InstanceManager::stringAddChar(uid, c);
    }

}