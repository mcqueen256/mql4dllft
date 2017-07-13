#ifndef MQLTYPES_HPP
#define MQLTYPES_HPP

namespace MQLTypes {
	enum ENUM_TIMEFRAMES {
		PERIOD_CURRENT,
		PERIOD_M1,
		PERIOD_M5,
		PERIOD_M15,
		PERIOD_M30,
		PERIOD_H1,
		PERIOD_H4,
		PERIOD_D1,
		PERIOD_W1,
		PERIOD_MN1
	};

	enum ENUM_ALIGN_MODE {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};

	struct MqlDateTime {
		int year;           // Year
		int mon;            // Month
		int day;            // Day
		int hour;           // Hour
		int min;            // Minutes
		int sec;            // Seconds
		int day_of_week;    // Day of week (0-Sunday, 1-Monday, ... ,6-Saturday)
		int day_of_year;    // Day number of the year (January 1st is assigned the number value of zero)
	};

	enum REASON {
		PROGRAM = 0, //Expert Advisor terminated its operation by calling the ExpertRemove() function
		REMOVE = 1, //Program has been deleted from the chart
		RECOMPILE = 2, //Program has been recompiled
		CHARTCHANGE = 3, //Symbol or chart period has been changed
		CHARTCLOSE = 4, //Chart has been closed
		PARAMETERS = 5, //Input parameters have been changed by a user
		ACCOUNT = 6, //Another account has been activated or reconnection to the trade server has occurred due to changes in the account settings
		TEMPLATE = 7, //A new template has been applied
		INITFAILED = 8, //This value means that OnInit() handler has returned a nonzero value
		CLOSE = 9 //Terminal has been closed
	};
}

#endif