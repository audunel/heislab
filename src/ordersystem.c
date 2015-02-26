#include "ordersystem.h"
#include "elev.h"
#include <stdio.h>

static int upOrder[4] = {0, 0, 0, 0};
static int downOrder[4] = {0, 0, 0, 0};
static int destination[4] = {0, 0, 0, 0};

void order(int up, int floor){
	if(up){
		upOrder[floor] = 1;
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
	}else {
		downOrder[floor] = 1;
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 1);
	}
}

void orderDestination(int floor){
	destination[floor] = 1;
	elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
}

int getOrder(int up, int floor){
	if(up){
		return upOrder[floor];
	}
	return downOrder[floor];
}

int getDestination(int floor){
	return destination[floor];
}

void removeOrder(int floor){
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
	
	printf("S, uporder %d%d%d downorder %d%d%d \n",  upOrder[0], upOrder[1], upOrder[2], downOrder[0], downOrder[1], downOrder[2]);
}

void clearAll(){
	for(int c = 0; c < 4; c++){
		removeOrder(c);
	}
}
