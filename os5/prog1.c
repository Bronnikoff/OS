#include "Massive.h"
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

//Бронников Максим
// Использование библиотеки во время исполнения программы при помощи подгрузки

#define EXPEREMENTAL_SIZE 10

int main(int argc, char const *argv[]) {
  Massive mass;
  void* handle = dlopen ("libworld.so", RTLD_LAZY);
  if (!handle) {
     fputs (dlerror(), stderr);
     exit(1);
 }
 void (*creater)(Massive*, size_t);
 creater = dlsym(handle, "create_massive");
  (*creater)(&mass, EXPEREMENTAL_SIZE);
double* (*getter)(Massive*, size_t);
getter = dlsym(handle, "get_elem_massive");
size_t (*lenghter)(Massive*);
lenghter = dlsym(handle, "lenght_massive");
  for(size_t i=0; i < (*lenghter)(&mass); i++){
    *((*getter)(&mass, i)) = i * 1.55;
  }
  *((*getter)(&mass, 0)) = 233.8993;
  void (*voider)(Massive*);
  voider = dlsym(handle, "print_massive");
  (*voider)(&mass);
  void (*resizer)(Massive*, size_t);
  resizer = dlsym(handle, "resize_massive");
  (*resizer)(&mass, EXPEREMENTAL_SIZE * 2);
  *((*getter)(&mass, EXPEREMENTAL_SIZE + 1)) = 98.76;
  (*voider)(&mass);
  voider = dlsym(handle, "delete_massive");
  (*voider)(&mass);
  return 0;
}
