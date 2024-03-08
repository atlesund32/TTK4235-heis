#pragma once
#include "elevator.h"
#include <time.h>

// typedef struct {
//     int floor;
//     bool up;
//     bool down;
// } Queue;

// array queue[
//     [floor, up, down],
//     [floor, up, down],
//     [floor, up, down],
//     [floor, up, down],
// ]

void elevator_go_to_destination(Elevator* myElevator, int* timer_started);
// void elevator_stop_destination(int destination, int last_floor);
void elevator_last_floor(Elevator* myElevator);
void checkIntermediateStops(Elevator* myElevator, int floor, int* timer_started, time_t* timer);
// void queue_init(Queue* queue);