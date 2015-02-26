#include "elev.h"
#include "ordersystem.h"
#include "fsm.h"
#include <stdio.h>
#include "time.h"

typedef enum {downwards, wait, upwards, stop, doorOpenUp, doorOpenDown} states;

static states state = upwards;

int stoptime;


states orderUp(int floor){
	printf("uporder %d%d%d downorder %d%d%d \n",  getOrder(1, 0), getOrder(1, 1), getOrder(1, 2), getOrder(0, 1), getOrder(0, 2), getOrder(0, 3));
	for (int c = floor + 1; c < 4; c++){
		printf("floor %d, %d, %d\n", floor, c, getOrder(0, c) || getOrder(1, c));
		if(getDestination(c) || getOrder(0, c) || getOrder(1, c)){
			printf("upwards\n");
			return upwards;
		}
	}
	for (int c = floor - 1; c >= 0; c--){
		printf("floor %d, %d, %d\n", floor, c, getOrder(0, c) || getOrder(1, c));
		if(getDestination(c) || getOrder(0, c) || getOrder(1, c)){
			printf("downwards\n");
			return downwards;
		}
	}
	printf("wait\n");
	return wait;
}

states orderDown(int floor){
	printf("uporder %d%d%d downorder %d%d%d \n",  getOrder(1, 0), getOrder(1, 1), getOrder(1, 2), getOrder(0, 1), getOrder(0, 2), getOrder(0, 3));
	for (int c = floor - 1; c >= 0; c--){
		printf("floor %d, %d, %d\n", floor, c, getOrder(0, c) || getOrder(1, c));
		if(getDestination(c) || getOrder(0, c) || getOrder(1, c)){
			printf("downwards\n");
			return downwards;
		}
	}
	for (int c = floor + 1; c < 4; c++){
		printf("floor %d, %d, %d\n", floor, c, getOrder(0, c) || getOrder(1, c)); 
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
			//elev_set_motor_direction(DIRN_DOWN);
		}else if(elev_get_floor_sensor_signal() < floor){
			printf("upwards\n");
			state = upwards;
			//elev_set_motor_direction(DIRN_UP);
		}else{
			stoptime = clock() + 3 * CLOCKS_PER_SEC;
		}
		order(up, floor);
		break;		
	default:
		order(up, floor);
	}
}

void destinationButtonPressed(int floor){
	switch (state){
	case wait:
		if(elev_get_floor_sensor_signal() > floor){
			state = downwards;
			elev_set_motor_direction(DIRN_DOWN);
		}else if(elev_get_floor_sensor_signal() < floor){
			state = upwards;
			elev_set_motor_direction(DIRN_UP);
		}else{
			stoptime = clock() + 3 * CLOCKS_PER_SEC;
		}
		orderDestination(floor);
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
		printf("%d up\n", floor);
		state = orderUp(floor);
		if (getDestination(floor) || getOrder(1, floor) || state != upwards){
			doorOpen(floor);
		}
		break;
	case downwards:
		printf("%d up\n", floor);
		state = orderDown(floor);
		if (getDestination(floor) || getOrder(0, floor) || state != downwards){
			doorOpen(floor);
		}
		break;
	}
}

void timeout(){
	elev_set_door_open_lamp(0);
	switch (state){
	case upwards:
		elev_set_motor_direction(DIRN_UP);
		break;
	case downwards:
		elev_set_motor_direction(DIRN_DOWN);
		break;
	}
}
