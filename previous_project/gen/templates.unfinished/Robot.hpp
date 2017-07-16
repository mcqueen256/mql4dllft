#ifndef ROBOT_HPP
#define ROBOT_HPP

class Robot {
private:
	mql::MQL4APIInstance mql4_api;
public:
	Robot(int param1);
	~Robot();
	void quote();
	mql::MQL4APIInstance& api();
};

#endif