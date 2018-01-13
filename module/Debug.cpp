#include "Debug.h"

using namespace std;

void hold(void) {
	extern bool is_holding;
	if (is_holding) {
		cin.sync();
		cin.get();
	} else {
		// cout << std::endl;
	}
	return;
}