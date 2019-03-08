#ifndef _BANK_H_
#define _BANK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include "message.h"

#define SUCCESS 1
#define NOT_MEMORY -1
#define NOT_ENOUGH_MONEY -2
#define NOT_CLIENT -3
#define RECEIVER_NOT_CLIENT -4


typedef struct _client {
    ID client;
    int sum;
}Client; //*Client

typedef struct _clientDB {
    Client clients;
    struct _clientDB* left;
    struct _clientDB* right;
} *ClientDB;

ClientDB ClientDBCreate(void);
void ClientAdd(ClientDB cDB, ID client);
void ClientDBPrint(ClientDB cDB);
Client* ClientFind(ClientDB cDB, ID clientint);
void ClientDBDestroy(ClientDB cDB);
bool equal(ID first, ID second);
bool more(ID first, ID second);

//void ClientDBSave(ClientDB cDB, int file);
//void ClientDBLoad(ClientDB cDB, int file);

void ClientAccIncrease(Client* cl, int sum);
void ClientAccDecrease(Client* cl, int sum);
int ClientHasEnoughMoney(Client* cl, int sum);
void MoneyPut(ID Clientint, int sum, ClientDB cDB);
int MoneyGet(ID Clientint, int sum, ClientDB cDB);
int MoneySend(ID clientSender, ID clientReceiver, int sum, ClientDB cDB);
int CheckAccount(ID client, ClientDB cDB);

#endif
