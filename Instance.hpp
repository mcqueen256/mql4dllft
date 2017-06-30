#pragma once
#include <map>

#include "Robot.hpp"

class Instance {
	static std::map<int, Robot*> robots;
public:
	static void push(Robot* robot);
	static Robot* at(int instance);
	static void erase(int instance);
};

