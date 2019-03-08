#include"structures.h"
// #define SPASE_SIZE 8

void Menu();
void Error(char* error);

//
// void print_space() {
//     clrscr();
//   	home();
//     putchar(' ');
//     putchar(' ');
//   	set_display_atrib(B_RED);
//    for(int i = 0; i < SPASE_SIZE; i++){
//        printf("  %d ", i);
//    }
//    printf(" \n");
//    resetcolor();
//    set_display_atrib(B_BLUE);
//             // 12345678901234567890123456789012345
//        printf("  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");// 1
//        resetcolor();
//        set_display_atrib(B_RED);
//        putchar('1');
//        resetcolor();
//        set_display_atrib(B_BLUE);
//        putchar(' ');
//   		 printf("│   │   │   │   │   │   │   │   │\n"); //2
//   		 printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n"); //3
//        resetcolor();
//        set_display_atrib(B_RED);
//        putchar('2');
//        resetcolor();
//        set_display_atrib(B_BLUE);
//        putchar(' ');
//        printf("│   │   │   │   │   │   │   │   │\n");//4
//        printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n"); //5
//        resetcolor();
//        set_display_atrib(B_RED);
//        putchar('3');
//        resetcolor();
//        set_display_atrib(B_BLUE);
//        putchar(' ');
//        printf("│   │   │   │   │   │   │   │   │\n"); //6
//        printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n"); //7
//        resetcolor();
//        set_display_atrib(B_RED);
//        putchar('4');
//        resetcolor();
//        set_display_atrib(B_BLUE);
//        putchar(' ');
//        printf("│   │   │   │   │   │   │   │   │\n"); //8
//        printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n"); //9
//        resetcolor();
//        set_display_atrib(B_RED);
//        putchar('5');
//        resetcolor();
//        set_display_atrib(B_BLUE);
//        putchar(' ');
//        printf("│   │   │   │   │   │   │   │   │\n"); //10
//        printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n"); //11
//        resetcolor();
//        set_display_atrib(B_RED);
//        putchar('6');
//        resetcolor();
//        set_display_atrib(B_BLUE);
//        putchar(' ');
//        printf("│   │   │   │   │   │   │   │   │\n"); //12
//        printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n"); //13
//        resetcolor();
//        set_display_atrib(B_RED);
//        putchar('7');
//        resetcolor();
//        set_display_atrib(B_BLUE);
//        putchar(' ');
//        printf("│   │   │   │   │   │   │   │   │\n"); //14
//        printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n"); //15
//        resetcolor();
//        set_display_atrib(B_RED);
//        putchar('8');
//        resetcolor();
//        set_display_atrib(B_BLUE);
//        putchar(' ');
//        printf("│   │   │   │   │   │   │   │   │\n"); //16
//        printf("  └───┴───┴───┴───┴───┴───┴───┴───┘\n"); //17
//   	resetcolor();
//   return;
// }

void Error(char* error) {
    perror(error);
    exit(1);
}

void Menu() {
    printf("1. Play\n");
    printf("2. Statistics\n");
    printf("3. Quit\n");
}




int main(){
char str[] = "serv";


struct sockaddr_un addr;
int socketDescriptor;

char nickname[MAX_SIZE];
printf("Enter your nickname, maximum 32 symbols:\n");
scanf("%s", nickname);
int cmd;




}

}
