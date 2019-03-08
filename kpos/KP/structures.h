#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/un.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>
#include <pthread.h>
#include <ctype.h>

typedef struct info {
    char nickname[33];
    unsigned int wins;
    unsigned int loses;
    unsigned int draws;
}Info;

typedef struct acc {
    Info player;
    int descriptor;
}Account;

typedef struct game {
    Account* player1;
    Account* player2;
    char* lobbyName;
}Game;

typedef struct turn {
    char type;
    int line;
    int column;
}Turn;


typedef struct hist {
    Info* rival;
    char result;
}hist;

typedef struct H {
    hist* history;
    int max;
    int current;
}History;
