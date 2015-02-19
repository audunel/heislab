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

    while (1) {
		if (elev_get_floor_sensor_signal() != -1){
			floorSignal(elev_get_floor_sensor_signal());
		}
		for (int c = 0; c < 4; c++){
			if (elev_get_button_signal(BUTTON_COMMAND, c)){
				destinationButton(c);
			}
		}
		for (int c = 0; c < 3; c++){
			if (elev_get_button_signal(BUTTON_CALL_UP, c)){
				orderButton(1, c);
			}
			if (elev_get_button_signal(BUTTON_CALL_DOWN, c + 1)){
				orderButton(0, c);
			}
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
