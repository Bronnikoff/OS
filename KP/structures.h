#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>
#include <sys/un.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h> 
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>
#include <pthread.h>
#include <ctype.h>

#define SIZE 8
#define B_MAGENTA 45 
#define B_BLUE 44
#define B_RED 41
#define B_GREEN 42

#define ESC "\033"

#define home() printf(ESC "[H")
#define cler() printf(ESC "[2J")
#define gotoxy(x, y) printf(ESC "[%d;%dH", y, x)
#define resetcolor() printf(ESC "[0m")
#define setcolor(color) printf(ESC "[%dm", color)

typedef struct info {
    char nickname[33];
    unsigned int wins;
    unsigned int loses;
    bool buzy;
}Info;

typedef struct acc {
    Info* player;
    int descriptor;
}Account;

typedef struct game {
    Account* player1;
    Account* player2;
    char* lobbyName;
}Game;

typedef struct turn {
    char type;
    int line[2];
    int column[2];
}Turn;


typedef struct hist {
    Info* rival;
    char result;
}hist;

typedef struct vector {
    Info* data;
    int buzy;
    int len;
} vector;

void vector_create(vector* vec, int len){
    vec->len = len;
    vec->buzy = 0;
    vec->data = malloc(sizeof(Info) * len);
    return;
}

int vector_buzy(vector* vec){
    return vec->buzy;
}
void vector_add(vector* vec, char elem[33]){
    if(vec->buzy < vec->len){
        for(int i = 0; i < 33; i++){
            vec->data[vec->buzy].nickname[i] = elem[i];
        }
        vec->data[vec->buzy].wins = 0;
        vec->data[vec->buzy].buzy = 0;
        vec->data[vec->buzy].loses = 0;
        vec->buzy++;
        return;
    }else{
        vec->len*=2;
        vec->data = realloc(vec->data, vec->len * sizeof(Info));
        for(int i = 0; i < 33; i++){
            vec->data[vec->buzy].nickname[i] = elem[i];
        }
        vec->data[vec->buzy].wins = 0;
        vec->data[vec->buzy].buzy = 0;
        vec->data[vec->buzy].loses = 0;
        vec->buzy++;
        return;
    }
}

Info* vector_find(vector* vec, char elem[33]){
    for(int i=0; i<vec->buzy; i++){
        for(int j = 0; j<33; j++){
            if(vec->data[i].nickname[j]!=elem[j] && elem[j] != '\0'){
                break;
            } else if(vec->data[i].nickname[j] == '\0' && elem[j] == '\0'){
                return &vec->data[i];
            } else if(j == 32){
                return &vec->data[i];
            }
        }

    }
    vector_add(vec, elem);
    return &vec->data[vec->buzy - 1];
}


