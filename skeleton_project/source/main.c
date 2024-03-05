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
    elevator_init(&myElevator, elevio_floorSensor(), -1);

    while(1){
        //update the last floor
        elevator_last_floor(&myElevator);

        //run elevator to its destination if it has one. Turn destination to -1 if destination is reached
        elevator_go_to_destination(&myElevator);

        //update the orders (read inputs)
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
                if(btnPressed){
                    myElevator.orders[f][b] = btnPressed;
                }
                
            }
        }

        // Update destination from hall- and cab orders. Cab orders have higher priority
        if(myElevator.destination == -1){
            updateELevatorDestination(&myElevator);
        }

        
        //if the elevator is at a floor and the floor has an order, the elevator will stop

        if(elevio_floorSensor() != -1){
            checkFloorOrders(&myElevator, elevio_floorSensor());
        }

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