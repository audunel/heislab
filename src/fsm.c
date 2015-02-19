#include "elev.h"
#include "ordersystem.h"
#include "fsm.h"

typedef enum {upwards, downwards, stop, wait, doorOpenUp, doorOpenDown} state;

static state s = upwards;


state orderUp(){
	for (int c = elev_get_floor_sensor_signal() + 1; c < 4; c++){
		if(getDestination(c) || getOrder(1, c)){
			elev_set_motor_direction(DIRN_UP);
			return upwards;
		}
	}
	elev_set_motor_direction(DIRN_DOWN);
	return downwards;
}

state orderDown(){
	for (int c = elev_get_floor_sensor_signal() - 1; c >= 0; c--){
		if(getDestination(c) || getOrder(0, c)){
			elev_set_motor_direction(DIRN_DOWN);
			return downwards;
		}
	}
	elev_set_motor_direction(DIRN_UP);
	return upwards;
}

void orderButton(int up, int floor){
	switch (s){
	default:
		order(up, floor);
	}
}

void destinationButton(int floor){
	switch (s){
	default:
		orderDestination(floor);
	}
}

void floorSignal(int floor){
	switch (s){
	case upwards:
		if (getDestination(floor) || getOrder(1, floor)){
			s = doorOpenUp;
			removeOrder(floor);
		}
		if(elev_get_floor_sensor_signal() == 3){
			elev_set_motor_direction(DIRN_DOWN);
			s = downwards;
			printf("ok");
		}
		break;
	case downwards:
		if (getDestination(floor) || getOrder(0, floor)){
			s = orderDown();
			removeOrder(floor);
		}
		if(elev_get_floor_sensor_signal() == 0){
			elev_set_motor_direction(DIRN_UP);
			s = upwards;
		}
		break;
	case doorOpenUp:
		s = orderUp(); // Uten timer.
		break;
	case doorOpenDown:
		s = orderDown();
		break;
	}
}
