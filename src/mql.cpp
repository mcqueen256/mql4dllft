#include <iostream>

void f();

int sum(int a, int b) {
	int* c = new int;
	*c = a;
	return *c + b;
}

int main2(int args, char* argv[]) {
	std::cout << sum(1, 2) << std::endl;
	return 0;
}