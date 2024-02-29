#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/elevator.h"
#include "driver/queue.h"

int main(){
    elevio_init();
    elevio_stopLamp(0);
    printf("\n------------------------------Test elevator------------------------------\n\n");
    printf("Press the stop button on the elevator panel to exit\n");

    while(elevio_floorSensor() == -1){
        //wait for the elevator to reach a floor
        elevio_motorDirection(DIRN_DOWN);
    }

    elevio_motorDirection(DIRN_STOP);
    Elevator myElevator;
    elevator_init(&myElevator, elevio_floorSensor(), elevio_floorSensor());

    while(1){
        //update the last floor
        elevator_last_floor(&myElevator);

        //run elevetor to its destination
        elevator_go_to_destination(myElevator.destination, myElevator.last_floor);

        //update the orders
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
                if(btnPressed){
                    myElevator.orders[f][b] = btnPressed;
                }
                
            }
        }

        //logic for the elevator to take the orders
        

        //if the elevator reaches a destination, remove the destination from the orders
        

        //if the stop button is pressed, the elevator will stop
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }

        //the elevator will stop and wait for 20 ms before checking again
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}