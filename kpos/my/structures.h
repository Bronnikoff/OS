#ifndef __TERM_EXAMPLE__
#define __TERM_EXAMPLE__
#define MAX_SIZE 32


#include <sys/types.h>
#include <sys/socket.h>
#include <stdbool.h>
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

/****************************************************************
*                          ГРАФИКА                              *
*****************************************************************/

#define ESC "\033"

//Format text
#define RESET 		0
#define BRIGHT 		1
#define DIM			2
#define UNDERSCORE	3
#define BLINK		4
#define REVERSE		5
#define HIDDEN		6

//Foreground Colours (text)

#define F_BLACK 	30
#define F_RED		31
#define F_GREEN		32
#define F_YELLOW	33
#define F_BLUE		34
#define F_MAGENTA 	35
#define F_CYAN		36
#define F_WHITE		37

//Background Colours
#define B_BLACK 	40
#define B_RED		41
#define B_GREEN		42
#define B_YELLOW	44
#define B_BLUE		44
#define B_MAGENTA 	45
#define B_CYAN		46
#define B_WHITE		47


#define home() 			printf(ESC "[H") //Move cursor to the indicated row, column (origin at 1,1)
#define clrscr()		printf(ESC "[2J") //lear the screen, move to (1,1)
#define gotoxy(x,y)		printf(ESC "[%d;%dH", y, x)
#define visible_cursor() printf(ESC "[?251")
//Set Display Attribute Mode	<ESC>[{attr1};...;{attrn}m
#define resetcolor() printf(ESC "[0m")
#define set_display_atrib(color) 	printf(ESC "[%dm",color)

/*********************************************************************
*                         СТРУКТУРЫ ДЛЯ ИГРы                         *
**********************************************************************/

typedef struct info {
    char nickname[MAX_SIZE];
    unsigned int wins;
    unsigned int loses;
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

// typedef struct turn {
//     char type;
//     int line;
//     int column;
// }Turn;
//
//
// typedef struct hist {
//     Info* rival;
//     char result;
// }hist;
//
// typedef struct H {
//     hist* history;
//     int max;
//     int current;
// }History;

typedef struct Massive{
  size_t len;
  size_t buzy;
  Info* data;
} Massive;

void create_massive(Massive* mass, size_t Lenght);
void delete_massive(Massive* mass);
void resize_massive(Massive* mass, size_t Lenght);
size_t lenght_massive(Massive* mass);
size_t buzy_massive(Massive* mass);
void pop_massive(Massive* mass, char elem[MAX_SIZE]);
Info* get_elem_massive(Massive* mass, size_t index);
bool empty_massive(Massive* mass);
Info* find_massive(Massive* mass, char find[MAX_SIZE]);

Info* find_massive(Massive* mass, char find[MAX_SIZE]){
  for(int i = 0; i < mass->buzy; i++){
    for(int j = 0; j < MAX_SIZE; j++){
      if(mass->data[i].nickname[j] == '\0' && find[j] == '\0'){
        return &mass->data[i];
      } else if(mass->data[i].nickname[j] != find[j]){
        break;
      } else if(j == MAX_SIZE - 1 && mass->data[i].nickname[j] == find[j]){
        return &mass->data[i];
      }
    }
  }
  pop_massive(mass, find);
  printf("New player was created!\n");
  return &mass->data[mass->buzy - 1];
}

void create_massive(Massive* mass, size_t Lenght){
  if(Lenght >= 0){
    mass->len = Lenght;
    mass->buzy = 0;
    mass->data = malloc(sizeof(Info) * Lenght);
    printf("Massive created!\n");
  } else{
    printf("Error! Wrong size of massive!\n");
    exit(EXIT_FAILURE);
  }
  return;
}

size_t buzy_massive(Massive* mass){
  return mass->buzy;
}

void pop_massive(Massive* mass, char elem[MAX_SIZE]){
  if(mass->buzy < mass->len){
    mass->data[mass->buzy].nickname = nickname;
    mass->data[mass->buzy].wins = mass->data[mass->buzy].loses = 0;
    ++mass->buzy;
  }else{
    mass->len*=2;
    mass->data = realloc(mass->data, sizeof(Info) * mass->len);
    printf("Massive was incrementered!\n");
    mass->data[mass->buzy].nickname = nickname;
    mass->data[mass->buzy].wins = mass->data[mass->buzy].loses = 0;
    ++mass->buzy;
  }
  printf("Elem was added!\n");
  return;
}

void delete_massive(Massive* mass){
  mass->len = 0;
  mass->buzy = 0;
  free(mass->data);
  mass->data = NULL;
}

void resize_massive(Massive* mass, size_t Lenght){
  if(Lenght == mass->len){
    printf("Its old size! Massive don`t resized!");
    return;
  }else if(Lenght > 0 && mass->len > 0){
    mass->data = realloc(mass->data, sizeof(Info) * Lenght);
    mass->len = Lenght;
    printf("Massive resized!\n");
    return;
  } else if(!Lenght && mass->len > 0){
    free(mass->data);
    mass->len = 0;
    printf("Massive resized and empty now!\n");
    return;
  } else if(Lenght > 0 && !mass){
    mass->data = malloc(sizeof(Info) * Lenght);
    mass->len = Lenght;
  } else{
    printf("Wrong size! Massive dont resized!\n");
    return;
  }
}

Info* get_elem_massive(Massive* mass, size_t index){
  if((0 <= index) && (index < mass->len)){
    return &mass->data[index];
  } else{
    printf("Wrong index!\n");
    exit(EXIT_FAILURE);
  }
}

size_t lenght_massive(Massive* mass){
  return mass->len;
}

bool empty_massive(Massive* mass){
  return mass->len;
}

#endif /*__TERM_EXAMPLE__*/
