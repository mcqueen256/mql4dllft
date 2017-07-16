#pragma once
#include <map>

#include "Robot.hpp"

typedef long RobotReferenceType;

class InstanceManager {
	static std::map<RobotReferenceType, Robot*> robots;
public:
	static void push(Robot* robot);
	static Robot* at(RobotReferenceType instance);
	static void erase(RobotReferenceType instance);
};