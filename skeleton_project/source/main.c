#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevator.h"

int main(){
    elevio_init();
    elevio_stopLamp(0);
    printf("\n----------Test elevator----------\n\n");
    printf("Press the stop button on the elevator panel to exit\n");


    Elevator myElevator;
    elevator_init(&myElevator, 0);

    myElevator.last_floor = elevio_floorSensor();

    while(elevio_floorSensor() == -1){
        //wait for the elevator to reach a floor
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);
    myElevator.last_floor = elevio_floorSensor();


    while(1){

        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
                myElevator.orders[f][b] = btnPressed;
            }
        }


        // for(int f = 0; f < N_FLOORS; f++){
        //     for(int b = 0; b < N_BUTTONS; b++){
        //         if(myElevator.requests[f][b]){
                    
        //         }
        //     }
        // }




        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }

        //the elevator will stop and wait for 20 ms before checking again
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}