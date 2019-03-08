#include "structures.h"

const unsigned int LIMIT = 3;


void Error(char* error) {
    perror(error);
    exit(1);
}

void Warning(char* warning) {
    perror(warning);
}


void AcceptPlayer(int socket, Account* player) {
    do{
      printf("Accept\n");
        player->descriptor = player->descriptor = accept(socket, NULL, NULL);
        if(player->descriptor == -1) {
            Warning("Accept error");
        }
    } while(player->descriptor == -1);
}

char* GenerateName(int cnt) {
    char game[] = "game";
    char* num = (char*)malloc(sizeof(char) * 33);
    int i = 0;
    while(cnt > 10) {
        int tmp = cnt % 10;
        num[i] = '0' + tmp;
        cnt /= 10;
        i++;
    }
    num[i] = '0' + cnt;
    char* res = (char*)malloc(sizeof(char) * 33);

    strcpy(res,game);

    int j = 4;
    while(i >= 0) {
        res[j] = num[i];
        i--;
        j++;
    }
    res[j] = '\0';
    free(num);
    return res;
}

/*************************************************
 *                   Win check                   *
 *************************************************/

int CheckWin(char** field, int x, int y, int row, int size,char check) {
    //char a = 'X';
    int cnt = 0;
    //int c1 = y - (row - 1);
    for(int c1 = x - (row - 1); c1 <= x ;c1++) {
        cnt = 0;
        if(c1 < 0 || c1 >= size) {
            continue;
        }
        for(int i = 0; i < row; i++) {
            if((c1 + i) >= size || field[c1 + i][y] != check) {
                break;
            }
            cnt++;
        }
        if(cnt == row) {
            return 1;
        }
    }
    return 0;
}

int CheckWin1(char** field, int x, int y, int row, int size, char check) {
    int cnt = 0;
    for(int c1 = y - (row - 1); c1 <= y ; ++c1) {
        cnt = 0;
        if(c1 < 0 || c1 >= size) {
            continue;
        }
        for(int i = 0; i < row; i++) {
            if((c1 + i) >= size || field[x][c1 + i] != check) {
                break;
            }
            cnt++;
        }
        if(cnt == row) {
            return 1;
        }
    }
    return 0;
}

int CheckWin2(char** field, int x, int y, int row, int size, char check) {
    int cnt = 0;
    //int c1 = row - 1;
    for(int c1 = row - 1; c1 >= 0; c1--) {
        cnt = 0;
        int x1 = x - c1;
        int y1 = y - c1;
        if( x1 < 0 || y1 < 0 || x1 >= size || y1 >= size) {
            continue;
        }

        for(int i = 0; i < row; ++i) {
            if((x1 + i) >= size || (y1 + i) >= size || field[x1 + i][y1 + i] != check) {
                break;
            }
            cnt++;
        }
        if(cnt == row) {
            return 1;
        }
    }
    return 0;
}

int CheckWin3(char** field, int x, int y, int row, int size, char check) {
    int cnt = 0;
    //int c1 = row - 1;
    for(int c1 = row - 1; c1 >= 0; c1--) {
        cnt = 0;
        int x1 = x + c1;
        int y1 = y - c1;
        if( x1 < 0 || y1 < 0 || x1 >= size || y1 >= size) {
            continue;
        }
        for(int i = 0; i < row; i++) {
            if((x1 - i) < 0 || (y1 + i) >= size || field[x1 - i][y1 + i] != check) {
                break;
            }
            cnt++;
        }
        if(cnt == row) {
            return 1;
        }
    }
    return 0;
}

int CheckWinner(char** field, int x, int y, int row, int size, char check) {
    if( CheckWin(field, x, y, row, size, check)  ||
        CheckWin1(field, x, y, row, size, check) ||
        CheckWin2(field, x, y, row, size, check) ||
        CheckWin3(field, x, y, row, size, check)   ) {
        return 1;
    }
    return 0;
}

/*************************************************
 *                 Game thread                   *
 *************************************************/




void FreeGame(Game* game) {
    free(game->player1);
    free(game->player2);
    free(game->lobbyName);
}

void* Thread(void* inf) {

    srand(time(NULL));
    Game* info = (Game*)inf;
    // создаем сокет для игры
    struct sockaddr_un addr;
    memset(&addr,0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,info->lobbyName);
    int lobby = socket(AF_UNIX, SOCK_STREAM,0);
    if(lobby == -1) {
        perror("Socket error\n");
        FreeGame(info);
        free(info);
        return NULL;
    }

    if(bind(lobby,(struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Binding error");
        FreeGame(info);
        free(info);
        return NULL;
    }
    if(listen(lobby,LIMIT) == -1) {
        perror("Listen error");
        FreeGame(info);
        free(info);
        return NULL;
    }

    int token;
    int d1 = accept(lobby, NULL, NULL);
    read(d1,&token, sizeof(int));
    if(token == 1) {
        info->player1->descriptor = d1;
    }
    else {
        info->player2->descriptor = d1;
    }
    d1 = accept(lobby,NULL,NULL);
    read(d1,&token, sizeof(int));
    if(token == 1) {
        info->player1->descriptor = d1;
    }
    else {
        info->player2->descriptor = d1;
    }

    if(info->player1->descriptor != -1 && info->player2->descriptor != -1) {

        write(info->player1->descriptor,&info->player2->player, sizeof(Info));
        write(info->player2->descriptor,&info->player1->player, sizeof(Info));

        int lot = 1;
        write(info->player1->descriptor,&lot, sizeof(int));
        lot = 0;
        write(info->player2->descriptor,&lot, sizeof(int));

        int size;
        char p1Mark, p2Mark;
        int rowToWin;

        read(info->player1->descriptor,&size, sizeof(int));
        read(info->player2->descriptor,&p2Mark, sizeof(char));

        if(p2Mark == 'X') {
            p1Mark = 'O';
        }
        else {
            p1Mark = 'X';
        }
        write(info->player1->descriptor,&p1Mark, sizeof(char));
        write(info->player2->descriptor,&size, sizeof(int));

        read(info->player2->descriptor,&rowToWin, sizeof(int));
        write(info->player1->descriptor,&rowToWin, sizeof(int));

        printf("Game started: %s vs. %s\n", info->player1->player.nickname, info->player2->player.nickname);
        printf("Field size: %d\n",size);
        printf("Row to win: %d\n",rowToWin);
        char** field = (char**)malloc(size * sizeof(char*));
        for(int i = 0; i < size; ++i) {
            field[i] = (char*)malloc(size * sizeof(char));
        }

        int turnCnt = 0;
        int descriptors[2];
        char marks[2];
        marks[0] = p1Mark;
        marks[1] = p2Mark;
        descriptors[0] = info->player1->descriptor;
        descriptors[1] = info->player2->descriptor;

        int random = rand() % 2;
        Turn turn;

        turn.type = 's';
        write(descriptors[turnCnt + random],&turn, sizeof(Turn));
        read(descriptors[turnCnt + random],&turn,sizeof(Turn));
        field[turn.line][turn.column] = marks[(turnCnt + random) % 2];

        turnCnt++;
        while(1) {
            turn.type = 't';
            write(descriptors[(turnCnt + random) % 2],&turn, sizeof(Turn));
            read(descriptors[(turnCnt + random) % 2],&turn,sizeof(Turn));
            field[turn.line][turn.column] = marks[(turnCnt + random) % 2];
            if(CheckWinner(field,turn.line,turn.column,rowToWin,size,marks[(turnCnt + random) % 2])) {
                turn.type = 'w';
                write(descriptors[(turnCnt + random) % 2],&turn, sizeof(Turn));
                turn.type = 'l';
                write(descriptors[(turnCnt + random + 1) % 2],&turn, sizeof(Turn));
                break;
            }
            turnCnt++;
            if(turnCnt == size * size) {
                turn.type = 'd';
                write(descriptors[(turnCnt + random) % 2],&turn, sizeof(Turn));
                turn.type = 'e';
                write(descriptors[(turnCnt + random + 1) % 2],&turn, sizeof(Turn));

            }

        }
        for(int i = 0; i < size;i++) {
            free(field[i]);
        }
        free(field);

    }
    else {
        printf("Failed to connect");
    }
    free(info->player1);
    free(info->player2);

    unlink(info->lobbyName);
    free(info->lobbyName);
    free(info);
    return NULL;
}

/*************************************************
 *                      MAIN                     *
 *************************************************/

int main() {
    char str[] = "serv"; //название сокета
    char rep1[] = "Waiting for second player...\n";
    char rep2[] = "Starting game. Connecting to the lobby.\n";
    char rep3[] = "Game is ready\n";
    //printf("%d\n", sizeof(str));
    //struct sockaddr server;

    struct sockaddr_un addr; //структура для инициализации сокета в bind
    int socketDescriptor; // дескриптор сокета

    socketDescriptor = socket(AF_UNIX, SOCK_STREAM, 0); // создаем(открываем) сокет
    if(socketDescriptor == -1)
        Error("Socket error");

    memset(&addr, 0, sizeof(struct sockaddr_un)); //обнуляем структуру(хз зачем)
    addr.sun_family = AF_UNIX; //указываем, что сокет подключеаем для внутренней работы в системе(не для интернета)

    strcpy(addr.sun_path, str); // имя открытого файла-обменника в системе задается как str
    if(bind(socketDescriptor, (struct sockaddr*)&addr, sizeof(addr)) == -1) //подключаем сокет в ОС
        Error("Binding error");
    if(listen(socketDescriptor, LIMIT) == -1) //говорим, что сокет готов принимать запросы(LIMIT запросов одновременно)
        Error("Listen error");
    unsigned int gameCnt = 0;
    int mod = 0;
    while(1) {
        Account* player1 = (Account*)malloc(sizeof(Account)); //переменные для хранения аккаунтов подключенных игргоков
        Account* player2 = (Account*)malloc(sizeof(Account));

        AcceptPlayer(socketDescriptor, player1); //ожидаем и принимаем дескриптор первого игрока
        if(read(player1->descriptor, &player1->player, sizeof(Info)) != sizeof(Info)) // принимаем информацию об игроке
            Error("Reading error");
        int sz = sizeof(rep1); //получаем размер записи об ожидании

        //printf("%d\n",sz);
        if(write(player1->descriptor, &sz, sizeof(int)) != sizeof(int)) //пишем размер записи rep1
            Error("Writing error");
        if(write(player1->descriptor, rep1, sz) != sz) //пишем саму эту запись
            Error("Writing error");


        AcceptPlayer(socketDescriptor, player2); //принимаем второго игрока
        if(read(player2->descriptor, &player2->player, sizeof(Info)) != sizeof(Info)) //считываем ег статистику
            Error("Reading error");

        sz = sizeof(rep3); //узнаем размер записи о начале игры
        if(write(player2->descriptor, &sz, sizeof(int)) != sizeof(int)) // и записываем его
            Error("Writing Error");

        if(write(player2->descriptor, rep3, sizeof(rep3)) != sizeof(rep3)) //записываем саму запись
            Error("Writing error");

        sz = sizeof(rep2);
        write(player1->descriptor, &sz, sizeof(int)); //пишем игрокам о начале игры
        write(player2->descriptor, &sz, sizeof(int));
        write(player1->descriptor, &rep2, sz);
        write(player2->descriptor, &rep2, sz);
        gameCnt++; //инкрементируем счетчик игроков

        char* name = GenerateName(gameCnt); //генерируем имя для игры
        int p = 1;
        sz = strlen(name); //пишем имя игры для подключения
        write(player1->descriptor, &sz, sizeof(int));
        write(player1->descriptor, name, sz * sizeof(char));
        write(player2->descriptor, &sz, sizeof(int));
        write(player2->descriptor, name, sz * sizeof(char));
        //пишем порядок их номеров в игры для определенияих ролей
        write(player1->descriptor, &p, sizeof(int));
        p = 2;
        write(player2->descriptor, &p, sizeof(int));
        // сохдаем структуру для хранения данных для игры
        Game* newGame = (Game*)malloc(sizeof(Game)); //инициализируем игру
        newGame->player1 = player1;
        newGame->player2 = player2;
        newGame->lobbyName = name;
        //закрываем дескрипторы игроков в этом процессе
        close(player1->descriptor);
        close(player2->descriptor);

        pthread_t thread; //создаем поток:
        pthread_create(&thread, NULL, Thread, newGame); //
        pthread_detach(thread); //отпускаем поток

    }
}
