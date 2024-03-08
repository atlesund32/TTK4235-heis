#include "elevator.h"
#include "elevio.h"
#include <stdlib.h>
#include <stdio.h>
void elevator_init(Elevator* myElevator, int start_floor, int dest, int d_o){
    myElevator->door_open = d_o;
    myElevator->destination = dest;
    myElevator->moving = 2;
    myElevator->last_floor = start_floor;

    for(int i = 0; i < N_FLOORS; i++) {
        for(int j = 0; j < N_BUTTONS; j++) {
            myElevator->orders[i][j] = 0;
        }
    }

}

void updateElevatorDestination(Elevator* myElevator) {
    int closestDistance = N_FLOORS;
    int destinationFloor = -1;
    int buttonNumber = -1;

    // loops through cab orders first to serve them with higher priority
    for(int f = 0; f < N_FLOORS; f++) {
        // Only serve cab orders
        if(myElevator->orders[f][BUTTON_CAB]) {
            int distance = abs(f - myElevator->last_floor);
            
            // If the order is closer than the current closest order, update the destination
            if(distance < closestDistance) {
                closestDistance = distance;
                destinationFloor = f;
            }
        }
    }
    // If there was a cab order, serve it
    if(destinationFloor != -1) {
        printf("To be removed: [%d][%d] \n", destinationFloor, buttonNumber);
        for(int i = 0; i < N_FLOORS; i++) {
            for(int j = 0; j < N_BUTTONS; j++) {
                printf("%d ", myElevator->orders[i][j]);
            }
            printf("\n");
        }
        myElevator->destination = destinationFloor;
        myElevator->orders[destinationFloor][buttonNumber] = 0; // Clears the order
        printf("Destination updated: %d\n", myElevator->destination);
        return; // Exits function if destination was found
    }

    // If there was no cab order, serve hall orders. The following logic is the same as the cab order logic
    else if (destinationFloor == -1) {
        for(int f = 0; f < N_FLOORS; f++) {
            for(int b = 0; b < N_BUTTONS; b++) {

                // Skip cab buttons
                if (b == BUTTON_CAB) continue;

                if(myElevator->orders[f][b]) {
                    int distance = abs(f - myElevator->last_floor);

                    if(distance < closestDistance) {
                        closestDistance = distance;
                        destinationFloor = f;
                        buttonNumber = b;
                    }
                }
            }
            
        }
        if(destinationFloor != -1 && buttonNumber != -1) {
                printf("To be removed: [%d][%d] \n", destinationFloor, buttonNumber);
                for(int i = 0; i < N_FLOORS; i++) {
                    for(int j = 0; j < N_BUTTONS; j++) {
                        printf("%d ", myElevator->orders[i][j]);
                    }
                    printf("\n");
                }
                myElevator->destination = destinationFloor;
                myElevator->orders[destinationFloor][buttonNumber] = 0; // Clears
                printf("Destination updated: %d\n this is zero: %d", myElevator->destination, myElevator->orders[destinationFloor][buttonNumber]);
        }
    }
    
}


