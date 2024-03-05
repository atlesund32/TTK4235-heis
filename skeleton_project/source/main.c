#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/elevator.h"
#include "driver/queue.h"
#include "driver/door.h"

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

    int timer_started = 0; // 0 = timer not started, 1 = timer started
    int timer = 0; //timer in seconds

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
        
        //Door functionality
        //NOTE TO SELF: door lamp should only be on while doing an order, not at start or end
        if(myElevator.destination == myElevator.last_floor && timer_started == 0 && myElevator.door_obstruction == 0){
            door_open(&myElevator, &timer_started, &timer);
        }
        if(timer >= 3 && timer_started == 1){
            door_close(&myElevator, &timer_started, &timer);
        }
        if(myElevator.door_open == 1 && elevio_obstruction() == 1){
            door_obstruction(&myElevator, &timer_started, &timer);
        }
        else{
            myElevator.door_obstruction = 0;
        }

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