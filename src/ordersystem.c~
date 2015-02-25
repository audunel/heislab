#include "ordersystem.h"
#include "elev.h"
#include <stdio.h>

static int upOrder[3] = {0, 0, 0};
static int downOrder[3] = {0, 0, 0};
static int destination[4] = {0, 0, 0, 0};

void order(int up, int floor){
	if(up){
		upOrder[floor] = 1;
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
	}else {
		downOrder[floor] = 1;
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor + 1, 1);
	}
	printf("uporder %d%d%d downorder %d%d%d \n",  upOrder[0], upOrder[1], upOrder[2], downOrder[0], downOrder[1], downOrder[2]);
}

void orderDestination(int floor){
	destination[floor] = 1;
	elev_set_button_lamp(BUTTON_COMMAND, floor, 1);
}

int getOrder(int up, int floor){
	if(up){
		return upOrder[floor];
	}
	return downOrder[floor - 1];
}

int getDestination(int floor){
	return destination[floor];
}
/*
void updateOrder(){
	for(int c = 0; c < 4; c++){
		if (elev_get_button_signal(BUTTON_COMMAND, c)){
			destination[c] = 1;
			elev_set_button_lamp(BUTTON_COMMAND, c, 1);
		}
	}
	for(int c = 0; c < 3; c++){
		if (elev_get_button_signal(BUTTON_CALL_UP, c)){
			upOrder[c] = 1;
			elev_set_button_lamp(BUTTON_CALL_UP, c, 1);
		}
		if (elev_get_button_signal(BUTTON_CALL_DOWN, c + 1)){
			downOrder[c] = 1;
			elev_set_button_lamp(BUTTON_CALL_DOWN, c + 1, 1);
		}
	}
}
*/
void removeOrder(int floor){
	elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
	if(floor != 3){
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
	}
	if(floor != 0){
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
	}
	
	switch(floor){
	case 0:
		destination[0] = 0;
		upOrder[0] = 0;
		break;
	case 1:
		destination[1] = 0;
		upOrder[1] = 0;
		downOrder[0] = 0;
		break;
	case 2:
		destination[2] = 0;
		upOrder[2] = 0;
		downOrder[1] = 0;
		break;
	case 3:
		destination[3] = 0;
		downOrder[2] = 0;
	}
	printf("S, uporder %d%d%d downorder %d%d%d \n",  upOrder[0], upOrder[1], upOrder[2], downOrder[0], downOrder[1], downOrder[2]);
}

void clearAll(){
	for(int c = 0; c < 4; c++){
		removeOrder(c);
	}
}
