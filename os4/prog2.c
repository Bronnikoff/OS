#include "Massive.h"
#define EXPEREMENTAL_SIZE 10

int main(int argc, char const *argv[]) {
  Massive mass;
  create_massive(&mass, EXPEREMENTAL_SIZE);
  for(size_t i=0; i < lenght_massive(&mass); i++){
    *(get_elem_massive(&mass, i)) = i * 1.55;
  }
  *(get_elem_massive(&mass, 0)) = 233.8993;
  print_massive(&mass);
  resize_massive(&mass, EXPEREMENTAL_SIZE * 2);
  *(get_elem_massive(&mass, EXPEREMENTAL_SIZE + 1)) = 98.76;
  print_massive(&mass);
  delete_massive(&mass);
  return 0;
}
