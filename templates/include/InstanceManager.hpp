#ifndef INSTANCEMANAGER_HPP
#define INSTANCEMANAGER_HPP

#include <map>

#include "RobotReferenceType.hpp"
#include "BaseRobot.hpp"

typedef long RobotReferenceType;

class InstanceManager {
	static std::map<RobotReferenceType, BaseRobot*> robots;
public:
	static void push(BaseRobot* robot);
	static BaseRobot* at(RobotReferenceType instance);
	static void erase(RobotReferenceType instance);
};

#endif