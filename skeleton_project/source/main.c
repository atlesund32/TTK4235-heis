#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "driver/elevator.h"
#include "driver/queue.h"
#include "driver/door.h"
#include "driver/lights.h"

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
    elevator_init(&myElevator, elevio_floorSensor(), -1, 0);
    clear_lights(&myElevator);

    int timer_started = 0; // 0 = timer not started, 1 = timer started
    time_t timer = time(NULL); //timer in seconds

    while(1){
        //update the last floor
        //Controls the last_floor of the object
        elevator_last_floor(&myElevator);
        order_lights(&myElevator);

        //update the orders (read inputs)
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                if(btnPressed && !myElevator.orders_processed[f][b]) {
                    myElevator.orders[f][b] = btnPressed;
                    myElevator.orders_processed[f][b] = 1; //order is processed
                    elevio_buttonLamp(f, b, btnPressed);
                    printf("floor: %d, button %d, moving: %d\n", f, b, myElevator.moving);
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
        elevator_last_floor(&myElevator);
        elevator_go_to_destination(&myElevator, &timer_started);

        int temp_floor = elevio_floorSensor();
        //if the elevator is at a floor and the floor has an order in the same direction, the elevator will stop
        if(temp_floor != -1){

            checkIntermediateStops(&myElevator, temp_floor, &timer_started, &timer);
        }

        //if the elevator reaches a destination, remove the destination from the orders
        elevator_last_floor(&myElevator);
        //Door functionality
        //NOTE TO SELF: door lamp should only be on while doing an order, not at start or end
        //Controls door lamp and timer
        // if(timer_started && (time(NULL) - timer) >= 3) {
        //     door_close(&myElevator, &timer_started, &timer);
        // }
        // if((myElevator.destination == myElevator.last_floor) && (myElevator.door_open == 0) && (timer_started == 0)) {
        //     printf("This is the destination\n");
        //     door_open(&myElevator, &timer_started, &timer);
        // }

        if(timer_started == 1) {
            if((time(NULL) - timer) >= 3 && (!elevio_obstruction()) && (myElevator.door_obstruction == 0)) {
                
                door_close(&myElevator, &timer_started, &timer);
            } else if(elevio_obstruction()) {
                myElevator.door_obstruction = 1;
                timer = time(NULL);
            }
        } else if((myElevator.destination == myElevator.last_floor) && !myElevator.door_open) {

            door_open(&myElevator, &timer_started, &timer);
        }
        if(myElevator.door_open == 0 || !elevio_obstruction()) {
            myElevator.door_obstruction = 0;
        }

       

        //if the stop button is pressed, the elevator will stop
        // if(elevio_stopButton()){
        //     elevio_motorDirection(DIRN_STOP);
        //     break;
        // }

        if(elevio_stopButton()){
            elevio_stopLamp(1);
            elevio_motorDirection(DIRN_STOP);
            myElevator.destination = -1;
            myElevator.moving = 2;
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    elevio_buttonLamp(f, b, 0);
                    myElevator.orders[f][b] = 0;
                    myElevator.orders_processed[f][b] = 0;
                    
                }
            }
            if(elevio_floorSensor() != -1){
                door_open(&myElevator, &timer_started, &timer);
            }
            //print orders array
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    printf("%d ", myElevator.orders[f][b]);
                }
                printf("\n");
            }

            while(elevio_stopButton()){
                //wait for the stop button to be released
            }
            if(elevio_floorSensor() != -1){
                door_open(&myElevator, &timer_started, &timer);
            }
            // timer_started = 0;
            myElevator.door_obstruction = 0;
            elevio_stopLamp(0);
        }
        
        
        //the elevator will stop and wait for 20 ms before checking again
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}