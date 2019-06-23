#include "structures.h"

void Error(char* error) {
    perror(error);
    exit(1);
}

char* GetStr(int* socketDescriptor) {
    int size;
    if(read(*socketDescriptor,&size, sizeof(int)) != sizeof(int)) //считываем размер записи
        Error("Reading error");

    char* reply = (char*)malloc((size + 1) * sizeof(char));
    if(read(*socketDescriptor,reply,size) != size) // считываем запись
        Error("Reading error");

    reply[size] = '\0';
    return reply; // возвращаем запись
}


/*************************************************
 *                      Game                     *
 *************************************************/
void PrintField(char** field) {
    cler();
    home();
    setcolor(B_MAGENTA);
    printf("\n   |");
    resetcolor();
    for(int i = 1; i < SIZE + 1; ++i){
        setcolor(B_MAGENTA);
        printf(" %d |", i);
    }
    putchar('\n');
    for(int i = 0; i < SIZE; ++i) {
        setcolor(B_MAGENTA);
        printf(" %d |", i + 1);
        resetcolor();
        setcolor(B_BLUE);
        for(int j = 0; j < SIZE; ++j) {
            printf(" %c ", field[i][j]);
            if(j != (SIZE - 1)) {
                putchar('|');
            }
        }
        printf("|\n");
        for(int j = 0; j < 4 * (SIZE + 1) - 1; j++) {
            putchar('-');
        }
        printf("-\n");
        resetcolor();
    }

   /* gotoxy(1, 3*SIZE);
    printf("                                                                      \n");
    printf("                                                                      \n");
    printf("                                                                      \n");
    printf("                                                                      \n");
    printf("                                                                      \n");
    gotoxy(1, 3*SIZE); */
}

void Print(Turn* turn, char mark){
    int c;
    if(mark == 'r'){
        c = B_RED;
    } else{
        c = B_GREEN;
    }
    home();
    if(turn->column[0] == turn->column[1]){
        int x = 6 + 4 * turn->column[0];
        int y = 3 + 2 * turn->line[0];
        gotoxy(x, y);
        setcolor(c);
        putchar(' ');
        resetcolor();
        y++;
        gotoxy(x, y);
        setcolor(c);
        putchar(' ');
        resetcolor();
        y++;
        gotoxy(x, y);
        setcolor(c);
        putchar(' ');
        resetcolor();
    } else{
        int x = 6 + 4 * turn->column[0];
        int y = 3 + 2 * turn->line[0];
        for(int i = 0; i < 5; i++){
        gotoxy(x, y);
        setcolor(c);
        putchar(' ');
        resetcolor();
        x++;
        }
    }
    gotoxy(1, 3*SIZE);
    return;    
}

int Incorrect(char** field, Turn* turn) {
    turn->line[0]--;
    turn->column[0]--;
    turn->line[1]--;
    turn->column[1]--;
    if(turn->line[0] < 0 || turn->line[0] >= SIZE || turn->column[0] < 0 || turn->column[0] >= SIZE) {
        printf("These aren't the droids you're looking for :)\n");
        printf("Try again\n");
        sleep(3);
        return 1;
    }
    if(turn->line[1] < 0 || turn->line[1] >= SIZE || turn->column[1] < 0 || turn->column[1] >= SIZE) {
        printf("These aren't the droids you're looking for :)\n");
        printf("Try again\n");
        sleep(3);
        return 1;
    }
    if(field[turn->line[0]][turn->column[0]] != ' ' || field[turn->line[1]][turn->column[1]] != ' ') {
        printf("These aren't the droids you're looking for :)\n");
        printf("This cage is busy. Try again\n");
        sleep(3);
        return 1;
    }
    if((turn->line[0] != turn->line[1]) && (turn->column[0] != turn->column[1])){
        printf("Wrong! Try again!\n");
        sleep(3);
        return 1;
    } else if((turn->column[0]-turn->column[1]!=1 && turn->column[1]-turn->column[0]!=1)&&(turn->line[1]-turn->line[0]!=1 && turn->line[0]-turn->line[1]!=1)){
        printf("Wrong! Try again!\n");
        sleep(3);
        return 1;
    }
    if(turn->column[0]-turn->column[1] == 1){
        int a = turn->column[0];
        turn->column[0]=turn->column[1];
        turn->column[1]=a;
    }
    if(turn->line[0]-turn->line[1] == 1){
        int a = turn->line[1];
        turn->line[1] = turn->line[0];
        turn->line[0] = a;
    }

    return 0; 
}

void GetTurn(char** field, Turn* turn) {
    printf("Your turn. Enter your choose!\n");
    do {

        gotoxy(1, 3*SIZE);
        printf("                                                                      \n");
        printf("                                                                      \n");
        printf("                                                                      \n");
        printf("                                                                      \n");
        printf("                                                                      \n");
        gotoxy(1, 3*SIZE);
        printf("Enter number line and colomn of first cage: ");
        scanf("%d%d",&turn->line[0],&turn->column[0]);
        printf("Enter number line and colomn of second cage: ");
        scanf("%d%d", &turn->line[1], &turn->column[1]);

    }while(Incorrect(field, turn));
}

void CheckConnection(int fd){
    char devnull  = 'o';
    read(fd, &devnull, 1);
    write(fd, &devnull, 1);
}



void PlayGame(char* name, Info* player, int token) {
    struct sockaddr_un addr1;
    int lobby;
    lobby = socket(AF_UNIX, SOCK_STREAM,0);
    if(lobby == -1) {
        perror("Socket error\n");
        free(name);
        return;
    }

    memset(&addr1,0, sizeof(struct sockaddr_un));
    addr1.sun_family = AF_UNIX;
    strcpy(addr1.sun_path,name);
    printf("Name of lobby: %s\n", name);
    if(connect(lobby,(struct sockaddr*)&addr1, sizeof(addr1)) == -1) {
        perror("Error when connecting to lobby");
        free(name);
        return;
    }

    char myMark, scndMark;
    int tmp = 1;
    Info* rival = (Info*)malloc(sizeof(Info));
    write(lobby,&token, sizeof(int));
    read(lobby,rival, sizeof(Info));
    int rg = rival->wins + rival->loses;
    printf("Your rival: %s %dG %dW %dL\n",rival->nickname,rg,rival->wins,rival->loses);
    read(lobby,&tmp, sizeof(int));
    if(tmp){
        myMark = 'g';
        scndMark = 'r';
        printf("Your colour is green!\n");
    } else{
        myMark = 'r';
        scndMark = 'g';
        printf("Your colour is red!\n");
    }
    sleep(5);
    char** field = (char**)malloc(SIZE * sizeof(char*));
    for(int i = 0; i < SIZE; ++i) {
        field[i] = (char*)malloc(SIZE * sizeof(char));
    }
    for(int i = 0; i < SIZE;i++) {
        for(int j = 0; j < SIZE; ++j) {
            field[i][j] = ' ';
        }
    }
    PrintField(field);
    Turn turn;
    while(1){
        CheckConnection(lobby);
        read(lobby,&turn, sizeof(Turn));
        if(turn.type == 't' || turn.type == 's') {
            if(turn.type != 's') {
                field[turn.line[0]][turn.column[0]] = 'x';
                field[turn.line[1]][turn.column[1]] = 'x';
                Print(&turn, scndMark);
                
            }
            GetTurn(field, &turn);
            Print(&turn, myMark);
            field[turn.line[0]][turn.column[0]] = 'x';
            field[turn.line[1]][turn.column[1]] = 'x';

            write(lobby,&turn, sizeof(Turn));
            
            printf("\n");
        }
        else if(turn.type == 'w'){
           cler();
           printf("======================================\n");
           printf("               You win!\n");
           printf("======================================\n");
            //res = 1;
           player->wins++;
           break;
        }
        else if(turn.type == 'l') {
            cler();
            field[turn.line[0]][turn.column[0]] = 'x';
            field[turn.line[1]][turn.column[1]] = 'x';
            printf("======================================\n");
            printf("               You lose!\n");
            printf("======================================\n");
            //res = -1;
            player->loses++;
            break;
        }
    }
    close(lobby);
    for(int i = 0; i < SIZE;i++) {
        free(field[i]);
    }
    free(field);
    free(name);
    name = NULL;
    //return res;
}

/*************************************************
 *                      MAIN                     *
 *************************************************/



void Menu() {
    printf("1. Play\n");
    printf("2. Statistics\n");
    printf("3. Quit\n");
}


int main() {
    char str[] = "serv";

    struct sockaddr_un addr;
    int socketDescriptor;



    Info player;
    player.loses = 0;
    player.wins = 0;
    printf("Enter your nickname, maximum 32 symbols:\n");
    scanf("%s",player.nickname);
    int cmd;
    while(1) {

        Menu();
        scanf("%d",&cmd);
        if(cmd == 1) {

            socketDescriptor = socket(AF_UNIX, SOCK_STREAM,0);
            if(socketDescriptor == -1)
                Error("Socket error");

            memset(&addr,0, sizeof(struct sockaddr_un));
            addr.sun_family = AF_UNIX;

            strcpy(addr.sun_path,str);

            if(connect(socketDescriptor,(struct sockaddr*)&addr, sizeof(addr)) == -1)
                perror("Error when connecting to the server");

            size_t sz = sizeof(char) * 33;
            if(write(socketDescriptor,&player.nickname, sz) != sz )
                Error("Sending error");

            if(read(socketDescriptor, &player, sizeof(Info))!=sizeof(Info)){
                Error("Info getting error");
            }
            if(player.buzy){
                printf("User with this name play now! Try connect again!\n");
                printf("Enter your nickname, maximum 32 symbols:\n");
                scanf("%s",player.nickname);
                continue;
            }
            char* reply = GetStr(&socketDescriptor);
            printf("%s",reply);
            free(reply);

            reply = GetStr(&socketDescriptor);
            printf("%s",reply);
            free(reply);

            char* name = GetStr(&socketDescriptor); //имя игры
            printf("name: %s\n", name);
            int p; //прядок игрока
            read(socketDescriptor,&p, sizeof(int));
            close(socketDescriptor);
            sleep(1);
            PlayGame(name, &player,p);
            cmd = 0;
            continue;
        }

        else if(cmd == 2) {
            if(!player.wins && !player.loses){
                printf("Sorry, Statistic will available after first game...\n");
                continue;
            }
            printf("Your statistics:\n");
            printf("Wins:  %d\n",player.wins);
            printf("Loses: %d\n",player.loses);
            cmd = 0;
            continue;
        }

        else if(cmd == 3) {
            break;
        }
    }
    close(socketDescriptor);
}