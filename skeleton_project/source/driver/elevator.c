#include "elevator.h"



void elevator_init(Elevator* myElevator, int start_floor, int dest){
    myElevator->destination = dest;
    myElevator->moving = 0;
    myElevator->last_floor = start_floor;

}

