#include "export.hpp"

extern "C" {
	/*
	 * Note: MQL calls are not available until after this function returns.
	 */
	MT4_API RobotReferenceType initialise(int param1) {
		try {
			BaseRobot* robot = new BaseRobot();
			InstanceManager::push(robot);
			return reinterpret_cast<RobotReferenceType>(robot);
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
			return 0;
		}
	}

	MT4_API void deinitialise(RobotReferenceType instance, const int reason) {
		/* Dealocate the robot memory. */
		delete InstanceManager::at(instance);
		/* Remove robot from existing instances. */
		InstanceManager::erase(instance);
	}

	MT4_API void enableTrading(RobotReferenceType instance) {

	}

	MT4_API void disableTrading(RobotReferenceType instance) {

	}

	MT4_API int bar(RobotReferenceType instance, char* time, double open, double high, double low, double close, double volume) {
		std::string string_time = time;
		auto event = [=] (std::function<void(void)> switchToDLLThread){
			InstanceManager::at(instance);
			switchToDLLThread();
		};
		return 0;
	}

	MT4_API int quote(RobotReferenceType instance, char* time, double open, double high, double low, double close, double volume) {
		return 0;
	}
}