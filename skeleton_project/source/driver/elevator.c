#include "elevator.h"



void elevator_init(Elevator* elevator, int start_floor){
    myElevator.destination = 0;
    myElevator.moving = 0;
    myElevator.last_floor = start_floor;

}