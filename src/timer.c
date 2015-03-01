#include "time.h"

static clock_t stopTime;

void timer_startTimer() {
	stopTime = clock() + 3 * CLOCKS_PER_SEC;
}

int timer_isTimeout() {
	return stopTime < clock();
}
