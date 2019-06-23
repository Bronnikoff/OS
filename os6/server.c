#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <signal.h>

#include <zmq.h>
#include "bank.h"
#include "message.h"

//Бронников Максим  Вариант: 1, 3, 2

volatile sig_atomic_t flag = 0;

void block_func(int sig)
{   int i;
    printf("Write what you want:\n0|Block/Unblock server\n1|Close server\nYour choice:");
    scanf("%d", &i);
    if(!i){
      if(flag){
        printf("Server blocked!\n");
        flag = 0;
      }else{
        printf("Server unblocked!\n");
        flag = 1;
      }
      signal(SIGINT, block_func);
      return;
    }else{
      printf("Close server!\n");
      exit(0);
    }
}

int main(void)
{
    int code;
    ClientDB clientBase = ClientDBCreate();
    void *context = zmq_ctx_new();
    void *responsSocket = zmq_socket(context, ZMQ_REP);

    char adress[41];
    printf("Enter bank's adress: ");
    ID bank;
    scanf("%s", bank.data);
    sprintf(adress, "%s%s", "tcp://*:", bank.data);
    printf("%s\n", adress);

    if(zmq_bind(responsSocket, adress)){
      printf("Error!\n");
      exit(0);
    }

    while (1) {
        signal(SIGINT, block_func);
        if (!flag) {

            zmq_msg_t message;

            zmq_msg_init(&message);
            zmq_msg_recv(&message, responsSocket, 0);
            MsgData *md = (MsgData *) zmq_msg_data(&message);
            zmq_msg_close(&message);

            char info[STR_SIZE];

            switch (md->action) {
                case 1: {
                    printf("Put money into the account id: %s\n", md->client.data);
                    MoneyPut(md->client, md->sum, clientBase);
                    ClientDBPrint(clientBase);
                    strcpy(info, "Operation was completed successfully\0");

                    memcpy(md->message, info, strlen(info) + 1);
                    zmq_msg_init_size(&message, sizeof(MsgData));
                    memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                    zmq_msg_send(&message, responsSocket, 0);
                    zmq_msg_close(&message);

                    break;
                }

                case 2: {
                    printf("Get money from the account id: %s\n", md->client.data);

                    code = MoneyGet(md->client, md->sum, clientBase);
                    if (code == SUCCESS) {
                        printf("Success\n");
                        ClientDBPrint(clientBase);
                        strcpy(info, "Operation was completed successfully\0");
                    } else if (code == NOT_ENOUGH_MONEY) {
                        printf("Not enough money\n");
                        strcpy(info, "You not enough money\0");
                    } else {
                        printf("Not bank client\n");
                        strcpy(info, "You aren't a client of bank\0");
                    }

                    memcpy(md->message, info, strlen(info) + 1);
                    zmq_msg_init_size(&message, sizeof(MsgData));
                    memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                    zmq_msg_send(&message, responsSocket, 0);
                    zmq_msg_close(&message);

                    break;
                }

                case 3: {
                    printf("Send money from account id: %s to account id: %s\n", md->client.data, md->receiverClient.data);
                    code = MoneySend(md->client, md->receiverClient, md->sum, clientBase);
                    if (code == SUCCESS) {
                        printf("Success\n");
                        ClientDBPrint(clientBase);
                        strcpy(info, "Operation was completed successfully\0");
                    } else if (code == NOT_ENOUGH_MONEY) {
                        printf("Not enought money\n");
                        strcpy(info, "You not enough money");
                    } else if (code == RECEIVER_NOT_CLIENT) {
                        printf("Receiver not bank client\n");
                        strcpy(info, "Receiver is not a client of bank\0");
                    }

                    memcpy(md->message, info, strlen(info) + 1);
                    zmq_msg_init_size(&message, sizeof(MsgData));
                    memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                    zmq_msg_send(&message, responsSocket, 0);
                    zmq_msg_close(&message);
                    break;
                }

                case 4: {
                    printf("Check account id: %s\n", md->client.data);
                    code = CheckAccount(md->client, clientBase);
                    if (code == NOT_CLIENT) {
                        printf("Not bank client\n");
                        strcpy(info, "You aren't client of bank\0");
                    } else {
                        printf("Client sum is %d\n", code);
                        ClientDBPrint(clientBase);
                        sprintf(info, "%s%d%c", "Your account is ", code, '\0');
                    }

                    memcpy(md->message, info, strlen(info) + 1);
                    zmq_msg_init_size(&message, sizeof(MsgData));
                    memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                    zmq_msg_send(&message, responsSocket, 0);
                    zmq_msg_close(&message);

                    break;
                }

            /* рудименты прошлых версий -- админка */
            /*case 11: {
                if (blocking) {
                    strcpy(info, "\0");
                } else {
                    printf("Save DB in %s \n", md->fileName);
                    int file = open(md->fileName, O_RDWR | O_CREAT, 0777);
                    ClientDBSave(clientBase, file);
                    strcpy(info, "DB is saved\0");
                }
                memcpy(md->message, info, strlen(info) + 1);
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                zmq_msg_send(&message, responsSocket, 0);
                zmq_msg_close(&message);
                break;
            }*/

            /*case 12: {
                if (blocking) {
                    strcpy(info, "\0");
                } else {
                    printf("Load DB from %s \n", md->fileName);
                    int file = open(md->fileName, O_RDWR | O_CREAT, 0777);
                    ClientDBDestroy(&clientBase);
                    clientBase = ClientDBCreate();
                    ClientDBLoad(clientBase, file);
                    strcpy(info, "DB is loaded\0");
                }
                memcpy(md->message, info, strlen(info) + 1);
                zmq_msg_init_size(&message, sizeof(MsgData));
                memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
                zmq_msg_send(&message, responsSocket, 0);
                zmq_msg_close(&message);
                break;
            }*/
            // case 11: {
            //     printf("Block server\n");
            //     //blocking = 1;
            //     strcpy(info, "Server is blocked\0");
            //     md->requester = responsSocket;
            //     memcpy(md->message, info, strlen(info) + 1);
            //     zmq_msg_init_size(&message, sizeof(MsgData));
            //     memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
            //     zmq_msg_send(&message, responsSocket, 0);
            //     zmq_msg_close(&message);
            //     block_func();
            //     break;
            // }
            // case 12: {
            //     printf("Unblock server\n");
            //     //blocking = 0;
            //     strcpy(info, "Server is unblocked\0");
            //     md->requester = responsSocket;
            //     memcpy(md->message, info, strlen(info) + 1);
            //     zmq_msg_init_size(&message, sizeof(MsgData));
            //     memcpy(zmq_msg_data(&message), md, sizeof(MsgData));
            //     zmq_msg_send(&message, responsSocket, 0);
            //     zmq_msg_close(&message);
            //     unblock_func();
            //     break;
            // }

            }
            zmq_msg_close(&message);
        }

    }
    zmq_close(responsSocket);
    zmq_ctx_destroy(context);

    ClientDBDestroy(clientBase);


}
