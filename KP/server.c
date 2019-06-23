#include "structures.h"
#define SAVENAME "save"
#define STARTLEN 12
#define str "serv"




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
        player->descriptor = accept(socket, NULL, NULL);
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


void Save(vector *vec){
    int fd = open(SAVENAME, O_RDWR|O_CREAT);
    for(int i = 0; i < vec->buzy; i++){
        vec->data[i].buzy = 0;
    }
    if(write(fd, &vec->len, sizeof(int))!=sizeof(int)){
        Error("Write error");
    }
    if(write(fd, &vec->buzy, sizeof(int))!=sizeof(int)){
        Error("Write error");
    }
    if(write(fd, vec->data, sizeof(Info)*vec->buzy) != sizeof(Info) * vec->buzy){
        Error("Save error");
    }
    return;
}

/*************************************************
 *                   Win check                   *
 *************************************************/


int CheckWin1(char** field) {
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE - 1; j++){
            if(field[i][j] == ' ' && field[i][j+1] == ' '){
                return 0;
            }
        }
    }
    return 1;
}

int CheckWin2(char** field) {
    for(int j = 0; j < SIZE; j++){
        for(int i = 0; i < SIZE - 1; i++){
            if(field[i][j] == ' ' && field[i+1][j] == ' '){
                return 0;
            }
        }
    }
    return 1;
}


int CheckWinner(char** field) {
    return (CheckWin1(field) && CheckWin2(field));
}

/*************************************************
 *                 Game thread                   *
 *************************************************/




void FreeGame(Game* game) {
    free(game->player1);
    free(game->player2);
    free(game->lobbyName);
}

void sighandler(int sig){}

bool CheckConnection(int fd){
    char devnull = 'o';
    signal(SIGPIPE, sighandler);
    write(fd, &devnull, 1);
    return read(fd, &devnull, 1);
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
   // char mark1, mark2;
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

    signal(SIGPIPE, sighandler);

    if(info->player1->descriptor != -1 && info->player2->descriptor != -1) {

        write(info->player1->descriptor,info->player2->player, sizeof(Info));
        signal(SIGPIPE, sighandler);

        write(info->player2->descriptor,info->player1->player, sizeof(Info));

        int lot = 1;
        
        write(info->player1->descriptor,&lot, sizeof(int));
        signal(SIGPIPE, sighandler);

        lot = 0;
        write(info->player2->descriptor,&lot, sizeof(int));
        signal(SIGPIPE, sighandler);


        char** field = (char**)malloc(SIZE * sizeof(char*));
        for(int i = 0; i < SIZE; ++i) {
            field[i] = (char*)malloc(SIZE * sizeof(char));
            for(int j = 0; j < SIZE; ++j){
                field[i][j] = ' ';
            }
        }

        int turnCnt = 0;
        int descriptors[2];
        descriptors[0] = info->player1->descriptor;
        descriptors[1] = info->player2->descriptor;

        int random = rand() % 2;
        Turn turn;

        turn.type = 's';
        if(CheckConnection(descriptors[turnCnt + random])){
        write(descriptors[turnCnt + random],&turn, sizeof(Turn));
        if(read(descriptors[turnCnt + random],&turn,sizeof(Turn)) < 1){
            if(CheckConnection(descriptors[(turnCnt + random + 1) % 2])){
            turn.type = 'w';
            write(descriptors[(turnCnt + random + 1) % 2], &turn, sizeof(Turn));
            if(turnCnt + random == 1){
                    info->player1->player->wins++;
                    info->player2->player->loses++;
                } else{
                    info->player1->player->loses++;
                    info->player2->player->wins++;
                }
            }
         close(lobby);
         info->player1->player->buzy = 0;
         info->player2->player->buzy = 0;
         free(info->player1);
         free(info->player2);
         unlink(info->lobbyName);
         free(info->lobbyName);
         free(info);
         return NULL;
        }
        } else{
            if(CheckConnection(descriptors[(turnCnt + random + 1) % 2])){
            turn.type = 'w';
            write(descriptors[(turnCnt + random + 1) % 2], &turn, sizeof(Turn));
            if(turnCnt + random == 1){
                    info->player1->player->wins++;
                    info->player2->player->loses++;
                } else{
                    info->player1->player->loses++;
                    info->player2->player->wins++;
                }
            }
         close(lobby);
         info->player1->player->buzy = 0;
         info->player2->player->buzy = 0;
         free(info->player1);
         free(info->player2);
         unlink(info->lobbyName);
         free(info->lobbyName);
         free(info);
         return NULL; 
        }

        field[turn.line[0]][turn.column[0]] = 'x';
        field[turn.line[1]][turn.column[1]] = 'x';


        turnCnt++;
        while(1) {
            turn.type = 't';
            if(CheckConnection(descriptors[(turnCnt + random) % 2])){
            write(descriptors[(turnCnt + random) % 2],&turn, sizeof(Turn));
            if(read(descriptors[(turnCnt + random) % 2],&turn,sizeof(Turn)) < 1){
                if(CheckConnection(descriptors[(turnCnt + random + 1) % 2])){
                turn.type = 'w';
                write(descriptors[(turnCnt + random + 1) % 2],&turn, sizeof(Turn));
                if((turnCnt + random)%2 == 0){
                    info->player1->player->wins++;
                    info->player2->player->loses++;
                } else{
                    info->player1->player->loses++;
                    info->player2->player->wins++;
                }
                }
                break;
            }
            } else{
                if(CheckConnection(descriptors[(turnCnt + random +1) % 2])){
                turn.type = 'w';
                write(descriptors[(turnCnt + random + 1) % 2],&turn, sizeof(Turn));
                if((turnCnt + random)%2 == 0){
                    info->player1->player->wins++;
                    info->player2->player->loses++;
                } else{
                    info->player1->player->loses++;
                    info->player2->player->wins++;
                }
                }
                break;
            }


            field[turn.line[0]][turn.column[0]] = 'x';
            field[turn.line[1]][turn.column[1]] = 'x';

            if(CheckWinner(field)){
                turn.type = 'w';
                if(CheckConnection(descriptors[(turnCnt+random) % 2])){
                    write(descriptors[(turnCnt + random) % 2],&turn, sizeof(Turn));
                }
                turn.type = 'l';
                if(CheckConnection(descriptors[(turnCnt+random+1) % 2])){
                    write(descriptors[(turnCnt + random + 1) % 2],&turn, sizeof(Turn));
                }
                if((turnCnt + random)%2 == 0){
                    info->player1->player->wins++;
                    info->player2->player->loses++;
                } else{
                    info->player1->player->loses++;
                    info->player2->player->wins++;
                }
                break;
            }
            turnCnt++;
        }
        for(int i = 0; i < SIZE;i++) {
            free(field[i]);
        }
        free(field);

    }
     
    close(lobby);
    info->player1->player->buzy = 0;
    info->player2->player->buzy = 0;
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

void* Menu(void* vec){
    vector* vec1 = (vector*) vec;
    signal(SIGINT, sighandler);
    printf("Welcome to play server!\nChoise:\n");
    printf("1-For making save and exit!\n");
    printf("2-For exit without save!\n");
    int choise;
    printf("Choice: ");
    scanf("%d", &choise);
    while(1){
    if(choise == 1){
        Save(vec1);
        unlink(str);
        exit(EXIT_SUCCESS);
    }else if(choise == 2){
        unlink(str);
        exit(EXIT_SUCCESS);
    }
    }
}

int main() {
    char rep1[] = "Waiting for second player...\n";
    char rep2[] = "Starting game. Connecting to the lobby.\n";
    char rep3[] = "Game is ready\n";

    vector* vec = malloc(sizeof(vector));
    vec->data = NULL;
    vector_create(vec, STARTLEN);
    char buf[33];

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
    pthread_t menu;
    pthread_create(&menu, NULL, Menu, (void*)vec);
    while(1) {
        Account* player1 = (Account*)malloc(sizeof(Account)); //переменные для хранения аккаунтов подключенных игргоков
        Account* player2 = (Account*)malloc(sizeof(Account));
        do{
        AcceptPlayer(socketDescriptor, player1); //ожидаем и принимаем дескриптор первого игрока
        if(read(player1->descriptor, &buf, sizeof(buf)) != sizeof(buf)) // принимаем информацию об игроке
            Error("Naming error");
        player1->player = vector_find(vec, buf);
        if(write(player1->descriptor, player1->player, sizeof(Info)) != sizeof(Info))
            Error("Writing error");  
        }while(player1->player->buzy); 
        

        int sz = sizeof(rep1); 

        //printf("%d\n",sz);
        if(write(player1->descriptor, &sz, sizeof(int)) != sizeof(int))
            Error("Writing error");
        if(write(player1->descriptor, rep1, sz) != sz) 
            Error("Writing error");
        player1->player->buzy = 1;

        do{
        AcceptPlayer(socketDescriptor, player2); 
        if(read(player2->descriptor, &buf, sizeof(buf)) != sizeof(buf)) 
            Error("Naming error");
        player2->player = vector_find(vec, buf);
        if(write(player2->descriptor, player2->player, sizeof(Info)) != sizeof(Info)) 
            Error("Writing error");   
        }while(player2->player->buzy);

        sz = sizeof(rep3); 
        if(write(player2->descriptor, &sz, sizeof(int)) != sizeof(int)) 
            Error("Writing Error");

        if(write(player2->descriptor, rep3, sizeof(rep3)) != sizeof(rep3)) 
            Error("Writing error");
        player2->player->buzy = 1;
        

        sz = sizeof(rep2);
        write(player1->descriptor, &sz, sizeof(int)); 
        write(player2->descriptor, &sz, sizeof(int));
        write(player1->descriptor, &rep2, sz);
        write(player2->descriptor, &rep2, sz);
        gameCnt++; 

        char* name = GenerateName(gameCnt); 
        int p = 1;
        sz = strlen(name); 
        write(player1->descriptor, &sz, sizeof(int));
        write(player1->descriptor, name, sz * sizeof(char));
        write(player2->descriptor, &sz, sizeof(int));
        write(player2->descriptor, name, sz * sizeof(char));
        
        write(player1->descriptor, &p, sizeof(int));
        p = 2;
        write(player2->descriptor, &p, sizeof(int));
        
        Game* newGame = (Game*)malloc(sizeof(Game)); 
        
        newGame->player1 = player1;
        newGame->player2 = player2;
        newGame->lobbyName = name;
        
        close(player1->descriptor);
        close(player2->descriptor);

        pthread_t thread; 
        pthread_create(&thread, NULL, Thread, newGame); 
        pthread_detach(thread); 

    }
}
