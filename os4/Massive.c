#include "Massive.h"
#include <errno.h>


void create_massive(Massive* mass, size_t Lenght){
  if(Lenght >= 0){
    mass->len = Lenght;
    mass->data = malloc(sizeof(double) * Lenght);
    printf("Massive created!\n");
  } else{
    printf("Error! Wrong size of massive!\n");
    exit(EXIT_FAILURE);
  }
  return;
}

void delete_massive(Massive* mass){
  mass->len = 0;
  free(mass->data);
  mass->data = NULL;
}

void resize_massive(Massive* mass, size_t Lenght){
  if(Lenght == mass->len){
    printf("Its old size! Massive don`t resized!");
    return;
  }else if(Lenght > 0 && mass->len > 0){
    mass->data = realloc(mass->data, sizeof(double) * Lenght);
    mass->len = Lenght;
    printf("Massive resized!\n");
    return;
  } else if(!Lenght && mass->len > 0){
    free(mass->data);
    mass->len = 0;
    printf("Massive resized and empty now!\n");
    return;
  } else if(Lenght > 0 && !mass){
    mass->data = malloc(sizeof(double) * Lenght);
    mass->len = Lenght;
  } else{
    printf("Wrong size! Massive dont resized!\n");
    return;
  }
}

double* get_elem_massive(Massive* mass, size_t index){
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

void print_massive(Massive* mass){
  printf("Massive:\n Elems:%ld \n|", mass->len);
  for (size_t i = 0; i < mass->len; i++) {
    printf("%lf|", mass->data[i]);
  }
  printf("|\n");
  return;
}
