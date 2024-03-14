#pragma once
#include "elevator.h"
#include <time.h>

void door_open(Elevator *myElevator, int *timer_started, time_t *timer);
void door_close(Elevator *myElevator, int *timer_started, time_t *timer);
void door_obstruction(Elevator *myElevator, int *timer_started, time_t *timer);