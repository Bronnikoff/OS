#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main() {
  int n;
  scanf("%d", &n);
  n=(1+n)*n/2;
  printf("%d\n", n);
  return 0;
}
