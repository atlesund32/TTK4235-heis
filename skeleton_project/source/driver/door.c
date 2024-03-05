#include "door.h"
#include "elevio.h"
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "elevator.h"

//door functionality
void door_open(Elevator* myElevator, int* timer_started, int* timer){
    myElevator->door_open = 1; //sets the door as open
    timer_started = 1; //start timer
    elevio_doorOpenLamp(1); //turn on the door lamp
    time_t timer = time(NULL); //start the timer
}

void door_close(Elevator* myElevator, int* timer_started, int* timer){
    myElevator->door_open = 0; //sets the door as closed
    elevio_doorOpenLamp(0); //turn off the door lamp
    timer = 0; //reset timer
    timer_started = 0; //stop timer
}

void door_obtruction(Elevator* myElevator, int* timer_started, int* timer){
    myElevator->door_obstruction = 1; //sets the door as obstructed
    timer = 0; //reset timer
    timer_started = 0; //stop timer

}