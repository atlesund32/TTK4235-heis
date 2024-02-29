#ifndef ELEVATOR_H //forhindrer dobbelt inkludering

typedef struct {
    int destination;
    int moving;
    int last_floor;
    array_t orders[N_FLOORS][N_BUTTONS]; //hall up. hall down, cab
} Elevator;


// orders[
//     [0, 0, 0],
//     [0, 0, 0],
//     [0, 0, 0],
//     [0, 0, 0]
//] 




void elevator_init(Elevator* elevator, int start_floor);


#endif