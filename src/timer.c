#include "time.h"

clock_t stopTime;

void startTimeout() {
	stopTime = clock() + 3 * CLOCKS_PER_SEC;
}

bool isTimeout() {
	return clock() < stopTime;
}
