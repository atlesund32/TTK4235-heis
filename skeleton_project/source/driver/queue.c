#include "queue.h"
#include "elevio.h"
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "door.h"

void elevator_go_to_destination(Elevator* myElevator){
    if(myElevator->destination == -1){
        return;
    } else{
        if(myElevator->destination > myElevator->last_floor && myElevator->destination != -1){
            elevio_motorDirection(DIRN_UP);
            myElevator->moving = 0;
        }
        else if(myElevator->destination < myElevator->last_floor && myElevator->destination != -1){
            elevio_motorDirection(DIRN_DOWN);
            myElevator->moving = 1;
        }
        else if(myElevator->destination == myElevator->last_floor && myElevator->destination != -1){
            elevio_motorDirection(DIRN_STOP);
            myElevator->destination = -1;
            myElevator->moving = 2;
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


void checkIntermediateStops(Elevator* myElevator, int floor, int* timer_started, time_t* timer){
    for(int b=0; b<N_BUTTONS; b++){
        if(myElevator->orders[floor][b] && timer_started == 0 && timer == 0){
            //Another if statement to check if the elevator is moving in the same direction as the person wants to go
            if((myElevator->moving == 0 )&& b == 0){
                door_open(myElevator, timer_started, timer);
                myElevator->orders[floor][b] = 0;
            }
            else if((myElevator->moving == 1) && b == 1){ 
                door_open(myElevator, timer_started, timer);
                myElevator->orders[floor][b] = 0;
            }
            if(b == 2){
                door_open(myElevator, timer_started, timer);
                myElevator->orders[floor][b] = 0;
                
            }
            
        }
    }
}