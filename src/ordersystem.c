#include "ordersystem.h"
#include "elev.h"
#include <stdio.h>

static int upOrder[4] = {0, 0, 0, 0};
static int downOrder[4] = {0, 0, 0, 0};
static int destination[4] = {0, 0, 0, 0};

void ordersystem_order(int up, int floor){ // For bestillinger "utenfor" heisen. up == 0 impliserer ned.
	if(up){
		upOrder[floor] = 1;
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
	}else {
		downOrder[floor] = 1;
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
	}
}

void ordersystem_orderDestination(int floor){ // For bestillinger "innenfor" heisen.
	destination[floor] = 1;
	elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
}

int ordersystem_getOrder(int up, int floor){
	if(up){
		return upOrder[floor];
	}
	return downOrder[floor];
}

int ordersystem_getDestination(int floor){
	return destination[floor];
}

void ordersystem_removeOrder(int floor){
	elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
	if(floor != 3){
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
	}
	if(floor != 0){
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
	}
	destination[floor] = 0;
	upOrder[floor] = 0;
	downOrder[floor] = 0;
}

void ordersystem_clearAll(){
	for(int c = 0; c < 4; c++){
		ordersystem_removeOrder(c);
	}
}
