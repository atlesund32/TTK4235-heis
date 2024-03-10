    #include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>

#include "elevio.h"
#include "con_load.h"

static int sockfd;
static pthread_mutex_t sockmtx;

//Setter opp forbindelse til heisserveren
void elevio_init(void){
    char ip[16] = "localhost";
    char port[8] = "15657";
    con_load("elevio.con",
        con_val("com_ip",   ip,   "%s")
        con_val("com_port", port, "%s")
    )
    
    pthread_mutex_init(&sockmtx, NULL);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1 && "Unable to set up socket");
    
    struct addrinfo hints = {
        .ai_family      = AF_INET, 
        .ai_socktype    = SOCK_STREAM, 
        .ai_protocol    = IPPROTO_TCP,
    };
    struct addrinfo* res;
    getaddrinfo(ip, port, &hints, &res);
    
    int fail = connect(sockfd, res->ai_addr, res->ai_addrlen);
    assert(fail == 0 && "Unable to connect to elevator server");
    
    freeaddrinfo(res);
    
    send(sockfd, (char[4]){0}, 4, 0);
}



//Setter motorretning
void elevio_motorDirection(MotorDirection dirn){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){1, dirn}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}

//Setter knappelyset
//floor = hvilken etasje lyset skal settes i
//button = hvilken knapp lyset skal settes på
//value = 1 setter på lyset
//value = 0 setter av lyset

void elevio_buttonLamp(int floor, ButtonType button, int value){
    assert(floor >= 0);
    assert(floor < N_FLOORS);
    assert(button >= 0);
    assert(button < N_BUTTONS);

    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){2, button, floor, value}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}

//Setter etasjelyset
void elevio_floorIndicator(int floor){
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){3, floor}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}

//Setter dørlampen
//value = 1 setter på dørlampen
//value = 0 setter av dørlampen
//value = 2 setter av dørlampen og døren åpen
//value = 3 setter av dørlampen og døren lukket
//value = 4 setter av dørlampen og døren i bevegelse
void elevio_doorOpenLamp(int value){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){4, value}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}

//Setter stopplampen
//value = 1 setter på stopplampen
//value = 0 setter av stopplampen
//value = 2 setter av stopplampen og døren åpen
//value = 3 setter av stopplampen og døren lukket
void elevio_stopLamp(int value){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){5, value}, 4, 0);
    pthread_mutex_unlock(&sockmtx);
}



//Gir oss om en knapp er trykket inn
//Returnerer 1 hvis knappen er trykket inn, 0 hvis ikke
int elevio_callButton(int floor, ButtonType button){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){6, button, floor}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1];
}

//Gir oss hvilken etasje heisen er i
//Returnerer -1 hvis heisen er mellom to etasjer
int elevio_floorSensor(void){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){7}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1] ? buf[2] : -1;
}

//Gir oss om stoppknappen er trykket inn
//Returnerer 1 hvis stoppknappen er trykket inn, 0 hvis ikke
int elevio_stopButton(void){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){8}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1];
}

//Gir oss om heisen er i nødstopp
//Returnerer 1 hvis heisen er i nødstopp, 0 hvis ikke
//Returnerer -1 hvis det er en feil
//Returnerer 2 hvis heisen er i nødstopp og døren er åpen
//Returnerer 3 hvis heisen er i nødstopp og døren er lukket
//Returnerer 4 hvis heisen er i nødstopp og døren er i bevegelse

int elevio_obstruction(void){
    pthread_mutex_lock(&sockmtx);
    send(sockfd, (char[4]){9}, 4, 0);
    char buf[4];
    recv(sockfd, buf, 4, 0);
    pthread_mutex_unlock(&sockmtx);
    return buf[1];
}
