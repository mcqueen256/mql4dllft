#include "EventThreader.hpp"
#include <sstream>

int main() {
	/* Example of most basic use */
	std::stringstream ss;
	auto f = [&ss](std::function<void(void)> switchToMainThread){
		for(int i = 0; i < 100; i++) { ss << "*"; }
		switchToMainThread();
		for(int i = 0; i < 50; i++) { ss << "*"; }
		switchToMainThread();
	};
	EventThreader et(f);
	et.switchToEventThread();
	for(int i = 0; i < 75; i++) { ss << "$"; }
	et.switchToEventThread();
	for(int i = 0; i < 25; i++) { ss << "$"; }
	et.join();

	/* Generate what the result should look like */
	std::string requirement;
	for(int i = 0; i < 100; i++) { requirement += "*"; }
	for(int i = 0; i < 75; i++) { requirement += "$"; }
	for(int i = 0; i < 50; i++) { requirement += "*"; }
	for(int i = 0; i < 25; i++) { requirement += "$"; }
	return (requirement == ss.str())?0:1;
}