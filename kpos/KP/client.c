#include "structures.h"

void Error(char* error) {
    perror(error);
    exit(1);
}

char* GetStr(int* socketDescriptor) {
    int size;
    if(read(*socketDescriptor,&size, sizeof(int)) != sizeof(int)) //считываем размер записи
        Error("Reading error");

    char* reply = (char*)malloc(size * sizeof(char));
    if(read(*socketDescriptor,reply,size) != size) // считываемм запись
        Error("Reading error");

    return reply; // возвращаем запись
}


/*************************************************
 *                      Game                     *
 *************************************************/
void PrintField(char** field, int size) {
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            printf(" %c ", field[i][j]);
            if(j != (size - 1)) {
                printf("|");
            }
        }
        printf("\n");
        if(i != (size - 1)) {
            for(int j = 0; j < 4 * size - 1; j++) {
                printf("-");
            }
            printf("\n");
        }
    }
}

int Incorrect(char** field, int size, Turn* turn) {
    turn->line--;
    turn->column--;
    if(turn->line < 0 || turn->line >= size || turn->column < 0 || turn->column >= size) {
        printf("These aren't the droids you're looking for :)\n");
        printf("Try again\n");
        return 1;
    }
    if(field[turn->line][turn->column] != ' ') {
        printf("These aren't the droids you're looking for :)\n");
        printf("This cage is busy. Try again\n");
        return 1;
    }

    return 0;
}

void GetTurn(char** field,int size,Turn* turn) {
    do {
        scanf("%d%d",&turn->line,&turn->column);
    }while(Incorrect(field,size,turn));
}

void UpdateHistory(History* history, Info* rival,char res) {
    if(history->current == history->max) {
        history->max *= 2;
        history->history = (hist*)realloc(history->history,(size_t)history->max);
    }
    history->history[history->current].rival = rival;
    history->history->result = res;
    history->current++;
}


void PlayGame(char* name, Info* player, int token, History* history) {
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
    if(connect(lobby,(struct sockaddr*)&addr1, sizeof(addr1)) == -1) {
        perror("Error when connecting to lobby");
        free(name);
        return;
    }

    char myMark;
    int tmp = 1;
    int size = 999;
    int rowToWin = 999;
    Info* rival = (Info*)malloc(sizeof(Info));
    write(lobby,&token, sizeof(int));
    read(lobby,rival, sizeof(Info));
    int rg = rival->wins + rival->loses + rival->draws;
    printf("Your rival: %s %dG %dW %dL %dD\n",rival->nickname,rg,rival->wins,rival->loses,rival->draws);
    read(lobby,&tmp, sizeof(int));

    if(tmp) {
        printf("Enter size:\n");
        do{
            if(size < 3) {
                printf("Wrong size, try again\n");
            }
            scanf("%d", &size);
        } while(size < 3);

        write(lobby, &size, sizeof(int));
        read(lobby,&myMark, sizeof(char));
        printf("Field size: %d\n",size);
        read(lobby,&rowToWin, sizeof(int));
    }
    else {
        printf("Choose your mark(X - x\\X, O - any other key)\n");
        scanf("%c%c",&myMark,&myMark);
        toupper(myMark);
        if(myMark != 'X') {
            myMark = 'O';
        }
        write(lobby, &myMark, sizeof(char));
        read(lobby, &size, sizeof(int));
        printf("Field size: %d\n",size);
        rowToWin = size;
        printf("Enter length of row, necessary to win:\n");
        do {
            if(rowToWin > size || rowToWin < 3) {
                printf("Wrong number, try again\n");
            }
            scanf("%d",&rowToWin);
        } while(rowToWin > size || rowToWin < 3);
        write(lobby,&rowToWin, sizeof(int));
    }
    printf("Your mark is: %c\n",myMark);
    printf("Length of row to win: %d\n",rowToWin);

    char** field = (char**)malloc(size * sizeof(char*));
    for(int i = 0; i < size; ++i) {
        field[i] = (char*)malloc(size * sizeof(char));
    }
    for(int i = 0; i < size;i++) {
        for(int j = 0; j < size; ++j) {
            field[i][j] = ' ';
        }
    }

    Turn turn;
    char rivalMark;
    if(myMark == 'X') {
        rivalMark = 'O';
    }
    else {
        rivalMark = 'X';
    }
    while(1){
        read(lobby,&turn, sizeof(Turn));
        if(turn.type == 't' || turn.type == 's') {
            if(turn.type != 's') {
                field[turn.line][turn.column] = rivalMark;
            }
            PrintField(field,size);
            printf("Your turn. Enter line and column of chosen cage:\n");
            GetTurn(field,size,&turn);
            field[turn.line][turn.column] = myMark;
            write(lobby,&turn, sizeof(Turn));
            PrintField(field,size);
            printf("\n");
        }
        else if(turn.type == 'w'){
            field[turn.line][turn.column] = myMark;
            PrintField(field,size);
            printf("You win!\n");
            //res = 1;
            player->wins++;
            break;
        }
        else if(turn.type == 'l') {
            field[turn.line][turn.column] = rivalMark;
            PrintField(field,size);
            printf("You lose :(\n");
            //res = -1;
            player->loses++;
            break;
        }
        else if(turn.type == 'd') {
            printf("Draw\n");
            //res = 0;
            player->draws++;
            break;
        }
        else if(turn.type == 'e') {
            field[turn.line][turn.column] = rivalMark;
            PrintField(field,size);
            printf("Draw\n");
            //res = 0;
            turn.type = 'd';
            player->draws++;
            break;
        }
    }
    close(lobby);
    for(int i = 0; i < size;i++) {
        free(field[i]);
    }
    free(field);
    free(name);
    UpdateHistory(history,rival,turn.type);
    //return res;
}

/*************************************************
 *                      MAIN                     *
 *************************************************/

void PrintHistory(History* history) {
    for(int i = history->current - 1; i >= 0; i--) {
        int tmp = history->history[i].rival->draws + history->history[i].rival->wins + history->history[i].rival->loses;
        printf("%c: %s %dG %dW %dL %dD\n",toupper(history->history[i].result), history->history[i].rival->nickname, tmp,
               history->history[i].rival->wins,history->history[i].rival->loses,history->history[i].rival->draws);
    }
}



void Menu() {
    printf("1. Play\n");
    printf("2. History\n");
    printf("3. Statistics\n");
    printf("4. Quit\n");
}


int main() {
    char str[] = "serv";

    struct sockaddr_un addr;
    int socketDescriptor;



    Info player;
    player.loses = 0;
    player.wins = 0;
    player.draws = 0;
    printf("Enter your nickname, maximum 32 symbols:\n");
    scanf("%s",player.nickname);

    History* history = (History*)malloc(sizeof(History));
    history->history = (hist*)malloc(sizeof(hist) * 100);
    history->max = 100;
    history->current = 0;
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

            size_t sz = sizeof(Info);
            if(write(socketDescriptor,&player, sz) != sz )
                Error("Sending error");

            char* reply = GetStr(&socketDescriptor);
            printf("%s",reply);
            free(reply);

            reply = GetStr(&socketDescriptor);
            printf("%s",reply);
            free(reply);

            char* name = GetStr(&socketDescriptor); //имя игры
            int p; //прядок игрока
            read(socketDescriptor,&p, sizeof(int));
            close(socketDescriptor);
            sleep(1);
            PlayGame(name, &player,p, history);
            cmd = 0;
            continue;
        }

        else if(cmd == 2) {
            PrintHistory(history);
            cmd = 0;
            continue;
        }

        else if(cmd == 3) {
            printf("Your statistics:\n");
            printf("Wins:  %d\n",player.wins);
            printf("Loses: %d\n",player.loses);
            printf("Draws: %d\n",player.draws);
            cmd = 0;
            continue;
        }

        else if(cmd == 4) {
            break;
        }
    }
    close(socketDescriptor);
    free(history->history);
    free(history);
}
