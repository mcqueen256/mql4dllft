#ifndef ROBOT_HPP
#define ROBOT_HPP

class Robot
{
public:
	Robot();
	~Robot();
	int onInit();
	void onDeinit(const int reason);
	void onTick();
	int OnCalculate(const int rates_total, const int prev_calculated);
};

#endif