#include "elev.h"
#include "ordersystem.h"
#include "fsm.h"

typedef enum {upwards, downwards, stop, wait, doorOpenUp, doorOpenDown} states;

static states state = upwards;


states orderUp(){
	for (int c = elev_get_floor_sensor_signal() + 1; c < 4; c++){
		if(getDestination(c) || getOrder(1, c)){
			elev_set_motor_direction(DIRN_UP);
			return upwards;
		}
	}
	elev_set_motor_direction(DIRN_DOWN);
	return downwards;
}

states orderDown(){
	for (int c = elev_get_floor_sensor_signal(); c >= 0; c--){
		if(getDestination(c) || getOrder(0, c)){
			elev_set_motor_direction(DIRN_DOWN);
			return downwards;
		}
	}
	elev_set_motor_direction(DIRN_UP);
	return upwards;
}

void orderButton(int up, int floor){
	switch (state){
	default:
		order(up, floor);
	}
}

void destinationButton(int floor){
	switch (state){
	default:
		orderDestination(floor);
	}
}

void floorSignal(int floor){
	switch (state){
	case upwards:
		if (getDestination(floor) || getOrder(1, floor)){
			state = doorOpenUp;
			removeOrder(floor);
			break;
		}
		if(elev_get_floor_sensor_signal() == 3){
			elev_set_motor_direction(DIRN_DOWN);
			state = downwards;
			printf("ok");
		}
		break;
	case downwards:
		if (getDestination(floor) || getOrder(0, floor)){
			state = orderDown();
			removeOrder(floor);
			break;
		}
		if(elev_get_floor_sensor_signal() == 0){
			elev_set_motor_direction(DIRN_UP);
			state = upwards;
		}
		break;
	case doorOpenUp:
		state = orderUp(); // Uten timer.
		break;
	case doorOpenDown:
		state = orderDown();
		break;
	}
}
