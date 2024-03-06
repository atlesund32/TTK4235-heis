#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/elevator.h"
#include "driver/queue.h"
#include "driver/door.h"
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
    elevator_init(&myElevator, elevio_floorSensor(), -1);

    int timer_started = 0; // 0 = timer not started, 1 = timer started
    int timer = 0; //timer in seconds

    while(1){
        //update the last floor
        //Controls the last_floor of the object
        elevator_last_floor(&myElevator);

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
        // Controls the destination of the elevator object
        if(myElevator.destination == -1){
            updateElevatorDestination(&myElevator);
        }

        // Run elevator to its destination if it has one. Doesn't react if destination is -1
        // Controls the motor direction of the elevator object
        elevator_go_to_destination(&myElevator);

        
        //if the elevator is at a floor and the floor has an order in the same direction, the elevator will stop
        if(elevio_floorSensor() != -1){
            checkIntermediateStops(&myElevator, elevio_floorSensor(), &timer_started, &timer);
        }

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
        //Door functionality
        //NOTE TO SELF: door lamp should only be on while doing an order, not at start or end
        //Controls door lamp and timer
        if(myElevator.destination == myElevator.last_floor && timer_started == 0 && myElevator.door_obstruction == 0){
            door_open(&myElevator, &timer_started, &timer);
        }
        if(timer_started == 1 && (time(NULL) - timer >= 3)){
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