#pragma once
#ifndef ELEVATOR_H //forhindrer dobbelt inkludering
#include "elevio.h"

typedef struct {
    int destination;
    int moving;
    int last_floor;
    int orders[N_FLOORS][N_BUTTONS]; //hall up. hall down, cab
    int door_open; //0 = closed, 1 = open
    int door_obstruction; //0 = no obstruction, 1 = obstruction
} Elevator;


// orders[
//     [0, 0, 0],
//     [0, 0, 0],
//     [0, 0, 0],
//     [0, 0, 0]
//] 




void elevator_init(Elevator* elevator, int start_floor, int dest);


#endif