#include <exception>
#include <map>
#include <iostream>
#include <string>
#include <cstdlib>

#include "InstanceManager.hpp"
#include "RobotReferenceType.hpp"
#include "BaseRobot.hpp"

std::map<RobotReferenceType, BaseRobot*> InstanceManager::robots;
std::map<int, std::string>               InstanceManager::strings;
std::mutex                               InstanceManager::stringLockMtx;

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

/**
 * Clams a string position in the strings map. This method is for thread and
 * probability safety.
 *
 * @return true if the UID is not in use, otherwise false
 */
bool InstanceManager::stringLock(int uid) {
	stringLockMtx.lock();
	// check that the uid is not already taken
	if (!stringAt(uid)) // not in the map
	{
		strings.insert(std::pair<int, std::string>(uid, ""));
		// the uid is now taken
		stringLockMtx.unlock();
		return true;
	}
	else // already exists
	{
		stringLockMtx.unlock();
		return false;
	}
}

/**
 * Returns a unique string reference.
 */
int InstanceManager::stringNewReference() {
	int ref;
	do {
		ref = std::rand();
	} while (!stringLock(ref));
	return ref;
}

/**
 * Add a single char to the string at uid. The string must exist as a clamed
 * string in strings before using this method.
 */
void InstanceManager::stringAddChar(int uid, char c) {
	strings.at(uid) += c;
}

/**
 * Get the string at this reference, then erase the string and reference hold in strings.
 */
std::string InstanceManager::stringGet(int uid) {
	std::string res = strings.at(uid);
	strings.erase(uid);
	return res;
}

/**
 * Returns true if ref is in the strings map.
 */
bool InstanceManager::stringAt(int ref) {
	std::map<int, std::string>::iterator it;
	it = strings.find(ref);
	return it != strings.end();
}

