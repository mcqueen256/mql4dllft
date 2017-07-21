#include <string>

#include "BaseRobot.hpp"

/*
 * The base robot 
 * The BaseRobot is an abstract class that requires some core methods to be
 * overriden (onQuote, onBar, onAnalysis) so that the robot can run correctly.
 * This class also stores some private objects like the buffers and MQL API. 
 */

/**
 *
 */
BaseRobot::BaseRobot(std::string robot_name) {

}

/**
 *
 */
BaseRobot::~BaseRobot() {

}


