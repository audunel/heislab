#include "time.h"

void sleep(int seconds) {
	clock_t temp;
	temp = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < temp) {};
}
