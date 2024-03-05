#include "queue.h"
#include "elevio.h"
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void elevator_go_to_destination(Elevator* myElevator){
    if(myElevator->destination == -1){
        return;
    } else{
        if(myElevator->destination > myElevator->last_floor){
            elevio_motorDirection(DIRN_UP);
        }
        else if(myElevator->destination < myElevator->last_floor){
            elevio_motorDirection(DIRN_DOWN);
        }
        else if(myElevator->destination == myElevator->last_floor){
            elevio_motorDirection(DIRN_STOP);
            myElevator->destination = -1;
        }
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

void checkFloorOrders(Elevator* myElevator, int floor){
    for(int b=0; b<N_BUTTONS; b++){
        if(myElevator->orders[floor][b]){
            elevio_motorDirection(DIRN_STOP);
            door_open();
            //NEEDS MORE CODE HERE LOGIC FOR 3 SECOND STOP AND DOOR LIGHT
        }
    }
}