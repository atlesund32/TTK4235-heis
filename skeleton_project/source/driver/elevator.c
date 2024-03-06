#include "elevator.h"
#include "elevio.h"
void elevator_init(Elevator* myElevator, int start_floor, int dest){
    myElevator->destination = dest;
    myElevator->moving = 0;
    myElevator->last_floor = start_floor;

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
        myElevator->destination = destinationFloor;
        myElevator->orders[destinationFloor][BUTTON_CAB] = 0; // Clears the order
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
                myElevator->destination = destinationFloor;
                myElevator->orders[destinationFloor][buttonNumber] = 0; // Clears
        }
    }
}


