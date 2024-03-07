#include "lights.h"
#include "elevator.h"

void order_lights(Elevator* myElevator){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            if(myElevator->orders[f][b]){
                elevio_buttonLamp(f, b, 1);
            }
            else{
                elevio_buttonLamp(f, b, 0);
            }
        }
    }
}

void clear_lights(Elevator* myElevator){
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            elevio_buttonLamp(f, b, 0);
        }
    }
}