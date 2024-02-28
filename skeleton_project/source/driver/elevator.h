#ifndef ELEVATOR_H //forhindrer dobbelt inkludering
#define ELEVATOR_H
#include <stdbool.h>

typedef struct {
    int destination;
    bool moving;
    int last_floor;
} Elevator;

void elevator_init(Elevator* elevator, int start_floor);


#endif