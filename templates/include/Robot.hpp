#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <string>

#include "BaseRobot.hpp"

class Robot : public BaseRobot{
private:
public:
	Robot(std::string name);
	~Robot();
};

#endif