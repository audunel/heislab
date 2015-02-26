#include "elev.h"
#include <stdio.h>
#include "fsm.h"


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);

    int floor = -1;
    int lastFloor;
    int upButton[4] = {0, 0, 0, 0};
    int lastUpButton[4];
    int downButton[4] = {0, 0, 0, 0};
    int lastDownButton[4];
    int destinationButton[4] = {0, 0, 0, 0};
    int lastDestinationButton[4];
    for(int c = 0; c < 3; c++){
    	printf("%d %d ", getOrder(0, c + 1), getOrder(1, c));
    }
    while (1) {
		lastFloor = floor;
		floor = elev_get_floor_sensor_signal();
		if (lastFloor == -1 && floor != -1){
			elev_set_floor_indicator(floor);
			floorSignal(floor);
		}
		for (int c = 0; c < 4; c++){
			lastDestinationButton[c] = destinationButton[c];
			destinationButton[c] = elev_get_button_signal(BUTTON_COMMAND, c);
			if (destinationButton[c] && !lastDestinationButton[c]){
				destinationButtonPressed(c);
			}
		}
		for (int c = 0; c < 3; c++){
			lastDownButton[c + 1] = downButton[c + 1];
			downButton[c + 1] = elev_get_button_signal(BUTTON_CALL_DOWN, c + 1);
			if (downButton[c + 1] && !lastDownButton[c + 1]){
				orderButton(0, c + 1);
			}
			lastUpButton[c] = upButton[c];
			upButton[c] = elev_get_button_signal(BUTTON_CALL_UP, c);
			if (upButton[c] && !lastUpButton[c]){
				orderButton(1, c);
			}
		}
		if (stoptime < clock()){
			timeout();
		}
        /* Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 2) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 1) {
            elev_set_motor_direction(DIRN_UP);
        }
		*/
         // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    return 0;
}
