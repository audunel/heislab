#include "time.h"

void sleep(int seconds) { // Denne trengs å endres.
	clock_t temp;
	temp = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < temp) {};
}
