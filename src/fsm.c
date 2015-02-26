#include "elev.h"
#include "ordersystem.h"
#include "fsm.h"
#include <stdio.h>
#include "time.h"

typedef enum {downwards, wait, upwards, stop, doorOpenUp, doorOpenDown} states;

static states state;
static int floorBelow; // Den nærmeste etasjen i retning ned.

int stoptime;



states orderUp(int floor){
	for (int c = floor + 1; c < 4; c++){
		if(getDestination(c) || getOrder(0, c) || getOrder(1, c)){
			printf("upwards\n");
			return upwards;
		}
	}
	for (int c = floor - 1; c >= 0; c--){
		if(getDestination(c) || getOrder(0, c) || getOrder(1, c)){
			printf("downwards\n");
			return downwards;
		}
	}
	printf("wait\n");
	return wait;
}

states orderDown(int floor){
	for (int c = floor - 1; c >= 0; c--){
		if(getDestination(c) || getOrder(0, c) || getOrder(1, c)){
			printf("downwards\n");
			return downwards;
		}
	}
	for (int c = floor + 1; c < 4; c++){
		if(getDestination(c) || getOrder(0, c) || getOrder(1, c)){
			printf("upwards\n");
			return upwards;
		}
	}
	printf("wait\n");
	return wait;
}

void orderButton(int up, int floor){
	switch (state){
	case wait:
		if(elev_get_floor_sensor_signal() > floor){
			printf("downwards\n");
			state = downwards;
			order(up, floor);
			if(stoptime < clock()){
				elev_set_motor_direction(DIRN_DOWN);
			}
		}else if(elev_get_floor_sensor_signal() < floor){
			printf("upwards\n");
			state = upwards;
			order(up, floor);
			if(stoptime < clock()){
				elev_set_motor_direction(DIRN_UP);
				floorBelow++;
			}
		}else{
			doorOpen(floor);
		}
		break;
	case stop:
		if(elev_get_floor_sensor_signal() != -1){
			if(elev_get_floor_sensor_signal() > floor){

				printf("downwards\n");
				state = downwards;
				order(up, floor);
				elev_set_door_open_lamp(0);

				if(stoptime < clock()){
					elev_set_motor_direction(DIRN_DOWN);
				}

			}else if(elev_get_floor_sensor_signal() < floor){

				elev_set_door_open_lamp(0);
				printf("upwards\n");
				state = upwards;
				order(up, floor);

				if(stoptime < clock()){
					floorBelow++;
					elev_set_motor_direction(DIRN_UP);
				}
			}else{
				doorOpen(floor);
				state = wait;
			}
		}else{
			order(up, floor);

			if(floorBelow >= floor){
				printf("downwards\n");
				state = downwards;
				elev_set_motor_direction(DIRN_DOWN);
			}else{
				printf("upwards\n");
				state = upwards;
				elev_set_motor_direction(DIRN_UP);
			}
		}
		break;
		
	default:
		order(up, floor);
	}
}

void destinationButtonPressed(int floor){
	switch (state){
	case wait:
		if(elev_get_floor_sensor_signal() > floor){
			printf("downwards\n");
			state = downwards;
			orderDestination(floor);
			if(stoptime < clock()){
				elev_set_motor_direction(DIRN_DOWN);
			}
		}else if(elev_get_floor_sensor_signal() < floor){
			printf("upwards\n");
			state = upwards;
			orderDestination(floor);
			if(stoptime < clock()){
				elev_set_motor_direction(DIRN_UP);
				floorBelow++;
			}
		}else{
			doorOpen(floor);
		}
		break;
	case stop:
		if(elev_get_floor_sensor_signal() != -1){
			if(elev_get_floor_sensor_signal() > floor){

				printf("downwards\n");
				state = downwards;
				orderDestination(floor);
				elev_set_door_open_lamp(0);

				if(stoptime < clock()){
					elev_set_motor_direction(DIRN_DOWN);
				}

			}else if(elev_get_floor_sensor_signal() < floor){

				elev_set_door_open_lamp(0);
				printf("upwards\n");
				state = upwards;
				orderDestination(floor);

				if(stoptime < clock()){
					floorBelow++;
					elev_set_motor_direction(DIRN_UP);
				}
			}else{
				doorOpen(floor);
				state = wait;
			}
		}else{
			orderDestination(floor);

			if(floorBelow >= floor){
				printf("downwards\n");
				state = downwards;
				elev_set_motor_direction(DIRN_DOWN);
			}else{
				printf("upwards\n");
				state = upwards;
				elev_set_motor_direction(DIRN_UP);
			}
		}
		break;
	
	default:
		orderDestination(floor);
	}
}

void doorOpen(int floor){
	elev_set_motor_direction(DIRN_STOP);
	stoptime = clock() + 3 * CLOCKS_PER_SEC;
	removeOrder(floor);
	elev_set_door_open_lamp(1);
}

void floorSignal(int floor){
	switch (state){
	case upwards:
		state = orderUp(floor);
		if (getDestination(floor) || getOrder(1, floor) || state != upwards){
			doorOpen(floor);
		}else{
			floorBelow++;
		}
		break;
	case downwards:
		state = orderDown(floor);
		if (getDestination(floor) || getOrder(0, floor) || state != downwards){
			doorOpen(floor);
		}
		floorBelow--;
		printf("\nfb %d\n", floorBelow);
		break;
	}
}

void timeout(){
	switch (state){
	case upwards:
		elev_set_motor_direction(DIRN_UP);
		floorBelow++;
		printf("\nfb %d\n", floorBelow);
		elev_set_door_open_lamp(0);
		break;
	case downwards:
		elev_set_motor_direction(DIRN_DOWN);
		elev_set_door_open_lamp(0);
		break;
	case wait:
		elev_set_door_open_lamp(0);
	}
}

void stopButtonPressed(){
	printf("floor below %d\n", floorBelow);
	elev_set_motor_direction(DIRN_STOP);
	state = stop;
	clearAll();
	if(elev_get_floor_sensor_signal() != -1) {
		elev_set_door_open_lamp(1);
	}
	elev_set_stop_lamp(1);
	while(elev_get_stop_signal()){}
	elev_set_stop_lamp(0);
}

void initialize(){
	int floor = elev_get_floor_sensor_signal();
	if(floor != -1){
		state = wait;
		elev_set_motor_direction(DIRN_STOP);
		floorBelow = floor - 1;
		printf("\nfb %d\n", floorBelow);
		return;
	}
	elev_set_motor_direction(DIRN_UP);
	while((floor = elev_get_floor_sensor_signal()) == -1){}
	state = wait;
	elev_set_motor_direction(DIRN_STOP);
	floorBelow = floor - 1;
	printf("\nfb %d\n", floorBelow);
}
