#ifndef INSTANCEMANAGER_HPP
#define INSTANCEMANAGER_HPP

#include <map>
#include <string>
#include <mutex>

#include "RobotReferenceType.hpp"
#include "Robot.hpp"

typedef long RobotReferenceType;

class InstanceManager {
	static std::map<RobotReferenceType, Robot*> robots;
	static std::map<int, std::string> strings;
	static std::mutex stringLockMtx;
public:
	static void push(Robot* robot);
	static Robot* at(RobotReferenceType instance);
	static void erase(RobotReferenceType instance);
	static bool stringLock(int uid);
	static int stringNewReference();
	static void stringAddChar(int uid, char c);
	static std::string stringGet(int uid);
	static bool stringAt(int ref);
};

#endif