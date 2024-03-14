#include "door.h"
#include "elevio.h"
#include <stdlib.h>
#include <signal.h>
#include "elevator.h"
#include <stdio.h>
#include "lights.h"

// door functionality
void door_open(Elevator *myElevator, int *timer_started, time_t *timer)
{

    elevio_motorDirection(DIRN_STOP);
    myElevator->moving = 2;

    myElevator->door_open = 1; // sets the door as open
    *timer_started = 1;        // start timer
    *timer = time(NULL);       // start the timer
    elevio_doorOpenLamp(1);    // turn on the door lamp
    printf("Door open, myEl->dest: %d, myEl->last floor: %d, timer_started: %d \n", myElevator->destination, myElevator->last_floor, *timer_started);
}

void door_close(Elevator *myElevator, int *timer_started, time_t *timer)
{
    elevio_doorOpenLamp(0);    // turn off the door lamp
    myElevator->door_open = 0; // sets the door as closed
    *timer = time(NULL);       // start the timer
    *timer_started = 0;        // stop timer
    printf("Door closed\n");
    if (elevio_floorSensor() == myElevator->destination)
    {
        myElevator->destination = -1; // reset destination
        printf("Destination removed: %d\n", myElevator->destination);

        // resets the orders_processed array
        for (int i = 0; i < N_BUTTONS; i++)
        {
            myElevator->orders_processed[myElevator->last_floor][i] = 0;
        }
    }
}

void door_obstruction(Elevator *myElevator, int *timer_started, time_t *timer)
{
    myElevator->door_obstruction = 1; // sets the door as obstructed
    *timer = time(NULL);              // reset timer
    *timer_started = 0;               // stop timer
}