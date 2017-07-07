#pragma once
#include <map>

#include "Robot.hpp"

class InstanceManager {
	static std::map<int, Robot*> robots;
public:
	static void push(Robot* robot);
	static Robot* at(int instance);
	static void erase(int instance);
};



class Instance {
private:
	Robot* robot;
public:
	Instance(Robot* robot);
	~Instance();
	

};