#include <iostream>

void f() {
	std::cout << "f()" << std::endl;
}

int sum(int a, int b) {
	int* c = new int;
	*c = a;
	return *c + b;
}

int main(int args, char* argv[]) {
	std::cout << sum(1, 2) << std::endl;
	return 0;
}