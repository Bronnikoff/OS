#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

//Бронников Максим, вариант №6

void ave();
char** readcom(short int* size);

char** readcom(short int* size){
  char c, buffer[15]; //временное хранилище аргумента
  char** str=NULL;
  ave();
  *size = 0;
  while(scanf("%s%c", buffer, &c)){//гениальное введение 'с' для остановки цикла
    str=(char**)realloc(str, ((*size)+1)*sizeof(char*));//память под новую ячейк
    str[(*size)]=(char*)malloc(strlen(buffer)*sizeof(char));//память под строку
    strcpy(str[(*size)++], buffer);//нельязя присваивать, тк работаем с адресами
    if(c=='\n'){//остановкка цикла
      break;
    }
  }
    return str;//возвращаем наш массив из строк обратно, легкий по памяти адрес
}

void ave(){
  printf("$ ");//обычное приглашение к вводу, можно было не делать отдельной
  return;//функции, но она была создана изначально с большим потенциалом
}//с целью создания более умного приглашения

int main(){
  printf("Welcome to OSlab1\nExersise №6:\n");
  short int i=0; //экономное хранение количества аргументов
  char c;//для преобразования, заданного заданием
  int j, river[2];
  char** str=NULL;//наш массив аргументов
  while(1){
    str=readcom(&i);//здесь получаем массив и его размер в параметре i
    if(strlen(str[0])==4&&str[0][0]=='e'&&str[0][1]=='x'&&str[0][2]=='i'&&str[0][3]=='t'){
      break;//очень глупая и примитивная, но нересурсоемкая остановка "оболочки"
    }//которая работает эффективно в силу ее простоты
    pipe(river);//создааем канал
    if(fork()!=0/*создание дочернего процесса*/){ //блок материнского процесса
      waitpid(-1, NULL, 0);//ожидаем конца дочернего процесса
      close(river[1]); //закрытие дескриптора, чтобы river[0] получил EOF
      while(read(river[0], &c, 1)/*пока можем считать один символ (до EOF)*/)
      { //здесь замена по варианту и печать в std::cout
        if(c==' '){
          putchar('_');
        }
        else if(c=='\t'){
          putchar('_');
          putchar('_');
        }
        else{
          putchar(c);
        }
      }
      close(river[0]);
    } else{ //начало блока для дочернего процесса
        dup2(river[1], 1);//заменяем системный std::cout на дескриптор river[1]
        execvp(str[0], str);//заменяем образ памяти для запуска команды оболочки
        printf("Error! Wrong command!\n");//если execvp не запустился, то пишем
        return 0;//иначе мы до сюда не дойдем
    }
    for(j=0; j<i; j++){
      free(str[j]);//освобождаем память всех ячеек со строками циклом for
      str[j]=NULL;
    }
    free(str);//освобождаем память массива ячеек для предотвращения утечек
    str=NULL;
  }
  printf("Made by Bronnikov Maksim (№1) M80-204b-17\n");
  return 0;
}
