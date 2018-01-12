#include <iostream>

using namespace std;

void hold(void) {
	extern bool is_holding;
	if (is_holding) {
		cin.sync();
		cin.get();
	}
	return;
}
