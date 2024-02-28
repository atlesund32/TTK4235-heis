#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevator.h"



int main(){
    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);
    Elevator myElevator;
    elevator_init(&myElevator, 0);

    while(1){
        //get floor
        int floor = elevio_floorSensor();

        //if the elevator is at a floor, stop the motor
        if(floor != -1){
            elevio_motorDirection(DIRN_STOP);
            myElevator.last_floor = floor;
        }

        //


        //the elevator will stop and wait for 20 ms before checking again
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
