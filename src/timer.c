#include "time.h"

static clock_t stopTime;

void startTimeout() {
	stopTime = clock() + 3 * CLOCKS_PER_SEC;
}

int isTimeout() {
	return clock() < stopTime;
}
