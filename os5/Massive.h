#ifndef  MASSIVE_H
#define MASSIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Massive{
  size_t len;
  double* data;
} Massive;

void create_massive(Massive* mass, size_t Lenght);
void delete_massive(Massive* mass);
void resize_massive(Massive* mass, size_t Lenght);
size_t lenght_massive(Massive* mass);
double* get_elem_massive(Massive* mass, size_t index);
bool empty_massive(Massive* mass);
void print_massive(Massive* mass);

#endif
