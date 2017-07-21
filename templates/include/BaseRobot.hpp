#ifndef BASEROBOT_HPP
#define BASEROBOT_HPP

#include <string>

class BaseRobot {
private:
	// mql::MQL4APIInstance mql4_api;
public:
	BaseRobot(std::string robot_name);
	~BaseRobot();
	// mql::MQL4APIInstance& api();
};

#endif