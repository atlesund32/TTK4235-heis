#include "queue.h"
#include "elevio.h"
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void elevator_go_to_destination(int destination, int last_floor){
    if(destination > last_floor){
        elevio_motorDirection(DIRN_UP);
    }
    else if(destination < last_floor){
        elevio_motorDirection(DIRN_DOWN);
    }
    else if(destination == last_floor){
        elevio_motorDirection(DIRN_STOP);
    }
}

void elevator_stop_destination(int destination, int last_floor){
    if(destination == last_floor){
        elevio_motorDirection(DIRN_STOP);
    }
}


void elevator_last_floor(Elevator* myElevator){
    if(elevio_floorSensor() != -1){
        myElevator->last_floor = elevio_floorSensor();
    }
}