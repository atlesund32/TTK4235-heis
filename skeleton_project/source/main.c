#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

int main(){
    elevio_init();

    printf("\n----------Test elevator----------\n\n");
    printf("Press the stop button on the elevator panel to exit\n");
    elevio_motorDirection(DIRN_DOWN); //hvilken vei motoren beveger seg
    
    while(1){

        //The elevator goes down and stops at a floor if i between floors at start
        int floor = elevio_floorSensor();
        if(floor != -1){
            elevio_motorDirection(DIRN_STOP);
        }



        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
    }

    return 0;
}

// int main(){
//     elevio_init(); //initialiserer heisen
    
//     printf("=== Example Program ===\n");
//     printf("Press the stop button on the elevator panel to exit\n");
//     elevio_motorDirection(DIRN_DOWN); //hvilken vei motoren beveger seg

//     while(1){
//         int floor = elevio_floorSensor(); //setter floor til der heisen befinner seg. Om heisen er i mellom to etg viser den (-1)

//         if(floor == 0){
//             elevio_motorDirection(DIRN_UP); //heisen går opp om i bånn etg
//         }

//         if(floor == N_FLOORS-1){
//             elevio_motorDirection(DIRN_DOWN); //heisen går ned om i topp etg
//         }


//         for(int f = 0; f < N_FLOORS; f++){ // f=floor
//             for(int b = 0; b < N_BUTTONS; b++){ // b=button
//                 int btnPressed = elevio_callButton(f, b); //btnPressed er knappen som blir trykket på
//                 elevio_buttonLamp(f, b, btnPressed); //lampen lyser
//             }
//         }

//         if(elevio_obstruction()){
//             elevio_stopLamp(1); //stoplampen lyser om obstruksjon
//         } else {
//             elevio_stopLamp(0);
//         }
        
//         if(elevio_stopButton()){
//             elevio_motorDirection(DIRN_STOP);
//             break;
//         }
        
//         nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
//     }

//     return 0;
// }
