#include "queue.h"
#include "elevio.h"
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "door.h"
#include "elevio.h"
#include <stdio.h>


void elevator_go_to_destination(Elevator* myElevator, int* timer_started){
    if(myElevator->destination == -1){
        return;
    } else{
        elevator_last_floor(myElevator);
        if((myElevator->destination > myElevator->last_floor) && (*timer_started == 0)){
            elevio_motorDirection(DIRN_UP);
            myElevator->moving = 0;
        }
        else if((myElevator->destination < myElevator->last_floor) && (*timer_started == 0)){
            elevio_motorDirection(DIRN_DOWN);
            myElevator->moving = 1;
        }

    }

}

// void elevator_stop_destination(int destination, int last_floor){
//     if(destination == last_floor){
//         elevio_motorDirection(DIRN_STOP);
        
//     }
// }


void elevator_last_floor(Elevator* myElevator){
    if(elevio_floorSensor() != -1){
        myElevator->last_floor = elevio_floorSensor();
    }
}


void checkIntermediateStops(Elevator* myElevator, int floor, int* timer_started, time_t* timer){
    
    for(int b=0; b<N_BUTTONS; b++){
        
        if((myElevator->orders[floor][b] == 1) && (*timer_started == 0)){
    
            //Another if statement to check if the elevator is moving in the same direction as the person wants to go
            if((myElevator->moving == 0 )&& b == 0){
                elevio_motorDirection(DIRN_STOP);
                myElevator->moving = 0;
                printf("Door open up\n");
                door_open(myElevator, timer_started, timer);
                myElevator->orders[floor][b] = 0;
            }
            else if((myElevator->moving == 1) && b == 1){ 
                elevio_motorDirection(DIRN_STOP);
                myElevator->moving = 1;
                printf("Door open down\n");
                door_open(myElevator, timer_started, timer);
                myElevator->orders[floor][b] = 0;
            }
            if(b == 2){
                elevio_motorDirection(DIRN_STOP);
                myElevator->moving = 2;
                printf("Door open cab\n");
                door_open(myElevator, timer_started, timer);
                myElevator->orders[floor][b] = 0;
                
            }
            
        }
    }
}
