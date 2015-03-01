#include "elev.h"
#include "ordersystem.h"
#include "fsm.h"
#include <stdio.h>
#include "time.h"

typedef enum {downwards, wait, upwards, stop} states;

static states state;
static int floorBelow; // Den nærmeste etasjen i retning ned.

int stoptime;


// fsm_getDirection1 brukes når heisen sist var på vei oppover. I motsatt tilfelle brukes getDirection2.

states fsm_getDirection1(int floor){
	for (int c = floor + 1; c < 4; c++){
		if(ordersystem_getDestination(c) || ordersystem_getOrder(0, c) || ordersystem_getOrder(1, c)){
			return upwards;
		}
	}
	for (int c = floor - 1; c >= 0; c--){
		if(ordersystem_getDestination(c) || ordersystem_getOrder(0, c) || ordersystem_getOrder(1, c)){
			return downwards;
		}
	}
	return wait;
}

states fsm_getDirection2(int floor){
	for (int c = floor - 1; c >= 0; c--){
		if(ordersystem_getDestination(c) || ordersystem_getOrder(0, c) || ordersystem_getOrder(1, c)){
			return downwards;
		}
	}
	for (int c = floor + 1; c < 4; c++){
		if(ordersystem_getDestination(c) || ordersystem_getOrder(0, c) || ordersystem_getOrder(1, c)){
			return upwards;
		}
	}
	return wait;
}


// fsm_orderButtonPressed er for knappene "utenfor" heisen. up == 0 impliserer at ned knappen har blitt trykket.

void fsm_orderButtonPressed(int up, int floor){ 
	switch (state){
	case wait:
		if(elev_get_floor_sensor_signal() > floor){

			state = downwards;
			ordersystem_order(up, floor);

			if(stoptime < clock()){
				elev_set_motor_direction(DIRN_DOWN);
			}
		}else if(elev_get_floor_sensor_signal() < floor){

			state = upwards;
			ordersystem_order(up, floor);

			if(stoptime < clock()){

				elev_set_motor_direction(DIRN_UP);
				floorBelow++;
			}
		}else{
			fsm_doorOpen(floor);
		}
		break;
	case stop:
		if(elev_get_floor_sensor_signal() != -1){
			if(elev_get_floor_sensor_signal() > floor){

				state = downwards;
				ordersystem_order(up, floor);
				elev_set_door_open_lamp(0);

				if(stoptime < clock()){
					elev_set_motor_direction(DIRN_DOWN);
				}

			}else if(elev_get_floor_sensor_signal() < floor){

				elev_set_door_open_lamp(0);
				state = upwards;
				ordersystem_order(up, floor);

				if(stoptime < clock()){
					floorBelow++;
					elev_set_motor_direction(DIRN_UP);
				}
			}else{
				fsm_doorOpen(floor);
				state = wait;
			}
		}else{
			ordersystem_order(up, floor);

			if(floorBelow >= floor){
				state = downwards;
				elev_set_motor_direction(DIRN_DOWN);
			}else{
				state = upwards;
				elev_set_motor_direction(DIRN_UP);
			}
		}
		break;
		
	default:
		ordersystem_order(up, floor);
	}
}


/*
fsm_destinationButtonPressed er for knappene "innenfor" heisen. Denne funksjonen er for øvrig identisk med fsm_orderButtonPressed med unntak av
at ordersystem_orderDestination kalles istedenfor order */
 
void fsm_destinationButtonPressed(int floor){
	switch (state){
	case wait:
		if(elev_get_floor_sensor_signal() > floor){

			state = downwards;
			ordersystem_orderDestination(floor);

			if(stoptime < clock()){
				elev_set_motor_direction(DIRN_DOWN);
			}
		}else if(elev_get_floor_sensor_signal() < floor){

			state = upwards;
			ordersystem_orderDestination(floor);

			if(stoptime < clock()){
				elev_set_motor_direction(DIRN_UP);
				floorBelow++;
			}
		}else{
			fsm_doorOpen(floor);
		}
		break;
	case stop:
		if(elev_get_floor_sensor_signal() != -1){

			if(elev_get_floor_sensor_signal() > floor){

				state = downwards;
				ordersystem_orderDestination(floor);
				elev_set_door_open_lamp(0);

				if(stoptime < clock()){
					elev_set_motor_direction(DIRN_DOWN);
				}

			}else if(elev_get_floor_sensor_signal() < floor){

				elev_set_door_open_lamp(0);
				state = upwards;
				ordersystem_orderDestination(floor);

				if(stoptime < clock()){
					floorBelow++;
					elev_set_motor_direction(DIRN_UP);
				}
			}else{
				fsm_doorOpen(floor);
				state = wait;
			}
		}else{
			ordersystem_orderDestination(floor);

			if(floorBelow >= floor){

				state = downwards;
				elev_set_motor_direction(DIRN_DOWN);
			}else{

				state = upwards;
				elev_set_motor_direction(DIRN_UP);
			}
		}
		break;
	
	default:
		ordersystem_orderDestination(floor);
	}
}

void fsm_doorOpen(int floor){
	elev_set_motor_direction(DIRN_STOP);
	stoptime = clock() + 3 * CLOCKS_PER_SEC;
	ordersystem_removeOrder(floor);
	elev_set_door_open_lamp(1);
}

void fsm_floorSignal(int floor){
	switch (state){
	case upwards:
		state = fsm_getDirection1(floor);
		if (ordersystem_getDestination(floor) || ordersystem_getOrder(1, floor) || state != upwards){

			fsm_doorOpen(floor);
		}else{
			floorBelow++;
		}
		break;
	case downwards:
		state = fsm_getDirection2(floor);

		if (ordersystem_getDestination(floor) || ordersystem_getOrder(0, floor) || state != downwards){
			fsm_doorOpen(floor);
		}

		floorBelow--;
		break;
	default:
		break;
	}
}

void fsm_timeout(){
	switch (state){
	case upwards:
		elev_set_motor_direction(DIRN_UP);
		floorBelow++;
		elev_set_door_open_lamp(0);
		break;
	case downwards:
		elev_set_motor_direction(DIRN_DOWN);
		elev_set_door_open_lamp(0);
		break;
	case wait:
		elev_set_door_open_lamp(0);
		break;
	default:
		break;
	}
}

void fsm_stopButtonPressed(){

	elev_set_motor_direction(DIRN_STOP);
	state = stop;
	ordersystem_clearAll();

	if(elev_get_floor_sensor_signal() != -1) {
		elev_set_door_open_lamp(1);
	}

	elev_set_stop_lamp(1);
	while(elev_get_stop_signal()){}
	elev_set_stop_lamp(0);
}

void fsm_initialize(){
	int floor = elev_get_floor_sensor_signal();
	if(floor != -1){

		state = wait;
		elev_set_motor_direction(DIRN_STOP);
		floorBelow = floor - 1;
		return;
	}
	elev_set_motor_direction(DIRN_UP);

	while((floor = elev_get_floor_sensor_signal()) == -1){}
	state = wait;
	elev_set_motor_direction(DIRN_STOP);
	floorBelow = floor - 1;
}
