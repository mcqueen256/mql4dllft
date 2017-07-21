#include <exception>
#include <map>
#include <iostream>

#include "InstanceManager.hpp"
#include "RobotReferenceType.hpp"
#include "BaseRobot.hpp"

std::map<RobotReferenceType, BaseRobot*> InstanceManager::robots;

void InstanceManager::push(BaseRobot * robot) {
	/* instance reference number */
	RobotReferenceType ref = (RobotReferenceType)(robot);
	/* check that it is not already in the map */
	if (robots.find(ref) != robots.end()) {
		throw std::invalid_argument("robot already in map");
	}
	/* add to the map */
	robots.insert(std::pair<RobotReferenceType, BaseRobot*>(ref, robot));
}

BaseRobot * InstanceManager::at(RobotReferenceType instance) {
	/* check that it is in the map */
	if (robots.find(instance) == robots.end()) {
		throw std::invalid_argument("robot not in map");
	}
	return robots[instance];
}

void InstanceManager::erase(RobotReferenceType instance) {
	robots.erase(instance);
}
