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

// Бронников Максим, вариант №3

void recursion(int* hustle){
  int status, p = *hustle;
  if (p<1){
    return;
  }
  --(*hustle);
  pid_t pid = fork();
  if(pid < 0){
    perror("fork error");
    exit(EXIT_FAILURE);
  } else if(pid == 0){
    recursion(hustle);
    exit(EXIT_SUCCESS);
  } else{
    waitpid(pid, &status, 0);
    if(WSTOPSIG(status)){
      exit(EXIT_FAILURE);
    }
    *hustle+=p;
    return;
  }
}

int main() {
  int n;
  printf("Welcom to lab 4\nEnter number: ");
  if(!scanf("%d", &n)){
    perror("Wrong value");
    exit(EXIT_FAILURE);
  } else if(n < 0){
    printf("Error! Value must be positive!\n");
    return 1;
  }
  int* hustle = (int*)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, 0, 0);
  if (hustle == MAP_FAILED){
    perror("mmap error");
    exit(EXIT_FAILURE);
  }
  *hustle=n;
  recursion(hustle);
  printf("Your anwser: %d\n", *hustle);
  if(munmap(hustle, 4)<0){
    perror("munmap");
    exit(EXIT_FAILURE);
  }
  return 0;
}
