#include "bank.h"


ClientDB ClientDBCreate(void)
{
    ClientDB cDB = (ClientDB) malloc(sizeof(*cDB));
    if (!cDB) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(NOT_MEMORY);
    }
    cDB->right = cDB->left = NULL;
    return cDB;
}

void ClientAdd(ClientDB cDB, ID client)
{
  if(cDB->clients.client.data[0] == '\0' && !cDB->clients.sum){
    cDB->clients.client = client;
    cDB->clients.sum = 0;
    return;
  }
  if(more(client, cDB->clients.client )){
    if( cDB->right != NULL){
      ClientAdd(cDB->right, client);
    }else{
      cDB->right = malloc(sizeof(*cDB));
      cDB->right->left = cDB->right->right = NULL;
      cDB->right->clients.client = client;
      cDB->right->clients.sum = 0;
    }}else if(more(cDB->clients.client, client)){
      if(cDB->left != NULL){
      ClientAdd(cDB->left, client);
    } else{
      cDB->left = malloc(sizeof(*cDB));
      cDB->left->left = cDB->left->right = NULL;
      cDB->left->clients.client = client;
      cDB->left->clients.sum = 0;
    }}
}

void ClientDBPrint(ClientDB cDB)
{
    if(cDB->left != NULL){
      ClientDBPrint(cDB->left);
    }
    printf("Client:%s\nSum:%d\n", cDB->clients.client.data, cDB->clients.sum);
    if(cDB->right != NULL){
      ClientDBPrint(cDB->right);
    }
    return;
}

bool more(ID first, ID second){
  for(int i=0; i<25; i++){
    if(first.data[i] > second.data[i]){
      return 1;
    } else if(first.data[i] < second.data[i]){
      return 0;
    }
    if(first.data[i] == '\0'){
      return 0;
    }
  }
  return 0;
}

bool equal(ID first, ID secnd){
  for(int i=0; i<25; i++){
    if(first.data[i]!=secnd.data[i]){
      return 0;
    }
    if(first.data[i] == '\0'){
      return 1;
    }
  }
  return 1;
}

Client* ClientFind(ClientDB cDB, ID clientch){
    if(cDB == NULL){
      return NULL;
    }
    if(!equal(cDB->clients.client, clientch)){
      if(more(cDB->clients.client, clientch)){
        return ClientFind(cDB->left, clientch);
      }else{
        return ClientFind(cDB->right, clientch);
      }
    }else{
      return (&cDB->clients);
    }
}

void ClientDBDestroy(ClientDB cDB)
{
    if(cDB->left != NULL){
      ClientDBDestroy(cDB->left);
    }
    if(cDB->right != NULL){
      ClientDBPrint(cDB->right);
    }
    free(cDB);
    return;
}

/*void ClientDBSave(ClientDB cDB, int file)
{
    if (file < 0) {
        printf("Cannot open file\n");
    } else {
        for (int i = 0; i < cDB->size; ++i) {
            write(file, &cDB->clients[i], sizeof(Client));
        }
    }
}
void ClientDBLoad(ClientDB cDB, int file)
{
    if (file < 0) {
        printf("Cannot open file\n");
    } else {
        Client cl;
        while (read(file, cl, sizeof(Client))) {
            MoneyPut(cl->client, cl->sum, cDB);
        }
    }
}*/

void ClientAccIncrease(Client* cl, int sum)
{
    cl->sum += sum;
}

void ClientAccDecrease(Client* cl, int sum)
{
    cl->sum -= sum;
}

int ClientHasEnoughMoney(Client* cl, int sum)
{
    return cl->sum >= sum;
}

void MoneyPut(ID clientch, int sum, ClientDB cDB)
{
    Client* cl = ClientFind(cDB, clientch);

    if (cl) {
        ClientAccIncrease(cl, sum);
    } else {
        ClientAdd(cDB, clientch);
        cl = ClientFind(cDB, clientch);
        ClientAccIncrease(cl, sum);
    }
}

int MoneyGet(ID clientch, int sum, ClientDB cDB)
{
    Client* cl = ClientFind(cDB, clientch);
    if (!cl) {
        return NOT_CLIENT;
    }
    if (ClientHasEnoughMoney(cl, sum)) {
        ClientAccDecrease(cl, sum);
        return SUCCESS;
    } else {
        return NOT_ENOUGH_MONEY;
    }
}

int MoneySend(ID clientSender, ID clientReceiver, int sum, ClientDB cDB)
{
    Client* clSender = ClientFind(cDB, clientSender);
    if (!clSender) {
        return NOT_CLIENT;
    }
    Client* clReceiver = ClientFind(cDB, clientReceiver);
    if (!clReceiver) {
        return RECEIVER_NOT_CLIENT;
    }

    if (ClientHasEnoughMoney(clSender, sum)) {
        ClientAccDecrease(clSender, sum);
        ClientAccIncrease(clReceiver, sum);
        return SUCCESS;
    } else {
        return NOT_ENOUGH_MONEY;
    }
}

int CheckAccount(ID client, ClientDB cDB)
{
    Client* cl = ClientFind(cDB, client);
    if (!cl) {
        return NOT_CLIENT;
    }
    return cl->sum;
}
