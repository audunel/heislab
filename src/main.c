#include "elev.h"
#include <stdio.h>
#include "fsm.h"
#include "timer.h"


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    fsm_initialize();

    static int floor = -1;
    static int lastFloor;
    static int upButton[4] = {0, 0, 0, 0};
    static int lastUpButton[4];
    static int downButton[4] = {0, 0, 0, 0};
    static int lastDownButton[4];
    static int destinationButton[4] = {0, 0, 0, 0};
    static int lastDestinationButton[4];
    static int timedOut = 1;
    static int lastTimedOut;

    while (1) {
		lastFloor = floor;
		floor = elev_get_floor_sensor_signal();
		if (lastFloor == -1 && floor != -1){
			elev_set_floor_indicator(floor);
			fsm_floorSignal(floor);
		}
		for (int c = 0; c < 4; c++){
			lastDestinationButton[c] = destinationButton[c];
			destinationButton[c] = elev_get_button_signal(BUTTON_COMMAND, c);
			if (destinationButton[c] && !lastDestinationButton[c]){
				fsm_destinationButtonPressed(c);
			}
		}
		for (int c = 0; c < 3; c++){
			lastDownButton[c + 1] = downButton[c + 1];
			downButton[c + 1] = elev_get_button_signal(BUTTON_CALL_DOWN, c + 1);
			if (downButton[c + 1] && !lastDownButton[c + 1]){
				fsm_orderButtonPressed(0, c + 1);
			}
			lastUpButton[c] = upButton[c];
			upButton[c] = elev_get_button_signal(BUTTON_CALL_UP, c);
			if (upButton[c] && !lastUpButton[c]){
				fsm_orderButtonPressed(1, c);
			}
		}
		lastTimedOut = timedOut;
		timedOut = timer_isTimeout();
		if (timedOut == 1 && lastTimedOut == 0){
			fsm_timeout();
		}
		if (elev_get_stop_signal()){
			fsm_stopButtonPressed();
		}
    }

    return 0;
}
