#ifndef BASEROBOT_HPP
#define BASEROBOT_HPP

#include <string>
#include <list>

#include "Quote.hpp"
#include "Bar.hpp"
#include "DataSet.hpp"

class BaseRobot {
private:
	// mql::MQL4APIInstance mql4_api;
public:
	BaseRobot(std::string robot_name);
	~BaseRobot();
	// mql::MQL4APIInstance& api();
	//std::list<Bar>& getBars();
	//std::list<DataSet>$ getDataSets();
	//Quote& getCurrentQuote();
};

#endif