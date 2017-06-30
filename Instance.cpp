#include <exception>
#include <map>
#include <iostream>

#include "Instance.hpp"
#include "Robot.hpp"

std::map<int, Robot*> Instance::robots;

void Instance::push(Robot * robot) {
	/* instance reference number */
	int ref = (int)(robot);
	/* check that it is not already in the map */
	if (robots.find(ref) != robots.end()) {
		throw std::invalid_argument("robot already in map");
	}
	/* add to the map */
	robots.insert(std::pair<int, Robot*>(ref, robot));
}

Robot * Instance::at(int instance) {
	/* check that it is in the map */
	if (robots.find(instance) == robots.end()) {
		throw std::invalid_argument("robot not in map");
	}
	return robots[instance];
}

void Instance::erase(int instance) {
	robots.erase(instance);
}
