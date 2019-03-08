#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

int main(int argc, char const *argv[]) {
  printf("Double:%ld\nFloat:%ld\n", sizeof(double), sizeof(float));
  printf("Calc: %d\n", 64/8);
  int massive[0];
  return 0;
}
