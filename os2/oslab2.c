#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

//Бронников Максим, вариант №3

void elitreader(int* n);
void elitwriter(int* n);

void elitreader(int* n){
  char buffer[10];
  read(0, buffer, 10);
  *n=atoi(buffer);
  return;
}

void elitwriter(int* n){
  char buffer[10];
  sprintf(buffer, "%d\n", *n);
  write(1, buffer, 10);
}

int main() {
  int S=0;
  int n=0;
  int river[2];
  pipe(river);
  elitreader(&n);
  dup2(river[0], 0);
if (n>1){
  if (fork()==0){
    --n;
    dup2(river[1], 1);
    elitwriter(&n);
    ++n;
    execl("./a.out", "./a.out", NULL);
  }
  else{
    waitpid(-1, NULL, 0);
    close(river[1]);
    elitreader(&S);
    S+=n;
    elitwriter(&S);
  }
}
else{
  S+=n;
  elitwriter(&S);
}
  return 0;
}
