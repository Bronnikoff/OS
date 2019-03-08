#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

 /* ________________
   |_0_|_1_|_2_|___|
  |_3_|_4_|_5_|___|
 |_6_|_7_|_8_|___|_______
|___|___|___|___|___|___|
   |___|_o_|_x_|___|___|
  |___|___|_o_|___|___|
 |___|___|___|_x_|___|
|___|___|___|___|___|
*/

#define SPASE_SIZE 3

typedef struct pthdata{
  short int pth_number;
  short int pth_i;
  bool player;// x ~ 1, 0 ~ o
  char* space;
  short int* priority;
} pthdata;

void* pth_function(void* params);
bool init_game(char* space, bool* player);
bool endgame(pthdata *data, short int itter);
bool defence(pthdata *data, short int itter);
void priority(pthdata* data, short int itter);
//begin ========================================================================
bool endgame(pthdata *data, short int itter){
  char c;
  bool m = 1;
  if(data->player){
    c='x';
  } else{
    c='o';
  }
  short int pth_i = data->pth_i+itter*data->pth_number;

  for (short int i = SPASE_SIZE*(pth_i/SPASE_SIZE); i < SPASE_SIZE+SPASE_SIZE*(pth_i/SPASE_SIZE); i++) {
    if((i==pth_i)||(data->space[i]==c)){
      continue;
    } else{
      m = 0;
      break;
    }
  }
  if(m){
    data->priority[itter]=14;
    return m;
  }
  m=1;
  for(short int i = pth_i%SPASE_SIZE; i<SPASE_SIZE * SPASE_SIZE; i+=SPASE_SIZE){
    if((i==pth_i)||(data->space[i]==c)){
      continue;
    } else{
      m=0;
      break;
    }
  }
  if(m){
    data->priority[itter]=14;
    return m;
  }
  if(pth_i % (SPASE_SIZE+1) == 0){
    m=1;
    for(short int i = 0; i<SPASE_SIZE*SPASE_SIZE; i+=(SPASE_SIZE+1)){
      if((i==pth_i)||(data->space[i]==c)){
        continue;
      } else{
        m=0;
        break;
      }
    }
    if(m){
      data->priority[itter]=14;
      return m;
    }
  }
  if(pth_i == (SPASE_SIZE-1 + (pth_i/SPASE_SIZE) * (SPASE_SIZE-1))){
    m=1;
    for(short int i=0; i<SPASE_SIZE; i++){
      if((pth_i==(SPASE_SIZE-1+i*(SPASE_SIZE-1)))||data->space[SPASE_SIZE-1+i*(SPASE_SIZE-1)]==c){
        continue;
      } else{
        m=0;
        break;
      }
    }
    if(m){
      data->priority[itter]=14;
      return m;
    }
  }
  return 0;
}
//------------------------------------------------------------------------------
//==============================================================================
bool defence(pthdata* data, short int itter){
  char c;
  bool m = 1;
  if(data->player){
    c='o';
  } else{
    c='x';
  }
  short int pth_i = data->pth_i+itter*data->pth_number;

  for (short int i = SPASE_SIZE*(pth_i/SPASE_SIZE); i < SPASE_SIZE+SPASE_SIZE*(pth_i/SPASE_SIZE); i++) {
    if((i==pth_i)||(data->space[i]==c)){
      continue;
    } else{
      m = 0;
      break;
    }
  }
  if(m){
    data->priority[itter]=13;
    return m;
  }
  m=1;
  for(short int i = pth_i%SPASE_SIZE; i<SPASE_SIZE * SPASE_SIZE; i+=SPASE_SIZE){
    if((i==pth_i)||(data->space[i]==c)){
      continue;
    } else{
      m=0;
      break;
    }
  }
  if(m){
    data->priority[itter]=13;
    return m;
  }
  if(pth_i % (SPASE_SIZE+1) == 0){
    m=1;
    for(short int i = 0; i<SPASE_SIZE*SPASE_SIZE; i+=(SPASE_SIZE+1)){
      if((i==pth_i)||(data->space[i]==c)){
        continue;
      } else{
        m=0;
        break;
      }
    }
    if(m){
      data->priority[itter]=13;
      return m;
    }
  }
  if(pth_i == (SPASE_SIZE-1 + (pth_i/SPASE_SIZE) * (SPASE_SIZE-1))){
    m=1;
    for(short int i=0; i<SPASE_SIZE; i++){
      if((pth_i==(SPASE_SIZE-1+i*(SPASE_SIZE-1)))||data->space[SPASE_SIZE-1+i*(SPASE_SIZE-1)]==c){
        continue;
      } else{
        m=0;
        break;
      }
    }
    if(m){
      data->priority[itter]=13;
      return m;
    }
  }
  return 0;
}
//------------------------------------------------------------------------------
//==============================================================================
void priority(pthdata* data, short int itter){
  char c, c1;
  short int m, m1;
  if(data->player){
    c='x';
    c1='o';
  } else{
    c='o';
    c1='x';
  }
  short int pth_i = data->pth_i+itter*data->pth_number;

  for (short int i = SPASE_SIZE*(pth_i/SPASE_SIZE); i < SPASE_SIZE+SPASE_SIZE*(pth_i/SPASE_SIZE); i++) {
    if(i==pth_i){
      continue;
    } else if(data->space[i]==c){
      m+=1;
    } else if(data->space[i]==c1){
      m1+=1;
    }
  }
  if(m>0 && m1==0){
    data->priority[itter]+=3;
  } else if(m1>0 && m==0){
    data->priority[itter]+=2;
  } else if(m1==0 && m == 0){
    data->priority[itter]+=1;
  }
  m = m1 = 0;
  for(short int i = pth_i%SPASE_SIZE; i<SPASE_SIZE * SPASE_SIZE; i+=SPASE_SIZE){
    if(i==pth_i){
      continue;
    } else if(data->space[i]==c){
      m+=1;
      continue;
    } else if(data->space[i]==c1){
      m1+=1;
    }
  }
  if(m>0 && m1==0){
    data->priority[itter]+=3;
  } else if(m1>0 && m==0){
    data->priority[itter]+=2;
  } else if(m1==0 && m == 0){
    data->priority[itter]+=1;
  }
  m = m1 =0;
  if(pth_i % (SPASE_SIZE+1) == 0){
    for(short int i = 0; i<SPASE_SIZE*SPASE_SIZE; i+=(SPASE_SIZE+1)){
      if(i==pth_i){
        continue;
      } else if(data->space[i]==c){
        m+=1;
        continue;
      } else if(data->space[i]==c1){
        m1+=1;
      }
    }
    if(m>0 && m1==0){
      data->priority[itter]+=3;
    } else if(m1>0 && m==0){
      data->priority[itter]+=2;
    } else if(m1==0 && m == 0){
      data->priority[itter]+=1;
    }
    m = m1 =0;
  }
  if(pth_i == (SPASE_SIZE-1 + (pth_i/SPASE_SIZE) * (SPASE_SIZE-1))){
    for(short int i=0; i<SPASE_SIZE; i++){
      if(pth_i==(SPASE_SIZE-1+i*(SPASE_SIZE-1))){
        continue;
      } else if(data->space[SPASE_SIZE-1+i*(SPASE_SIZE-1)]==c){
        m+=1;
        continue;
      } else if(data->space[SPASE_SIZE-1+i*(SPASE_SIZE-1)]==c1){
        m1+=1;
      }
    }
    if(m>0 && m1==0){
      data->priority[itter]+=3;
    } else if(m1>0 && m==0){
      data->priority[itter]+=2;
    } else if(m1==0 && m == 0){
      data->priority[itter]+=1;
    }
  }
  return ;
}
//------------------------------------------------------------------------------
//begin=========================================================================
void* pth_function(void* params){
  pthdata *data=(pthdata*)params;
  short int itter=0;
  data->priority=(short int*)malloc(((SPASE_SIZE*SPASE_SIZE)/data->pth_number + (data->pth_i < SPASE_SIZE*SPASE_SIZE%data->pth_number)) * sizeof(short int));
  while(data->pth_i+itter*data->pth_number<SPASE_SIZE*SPASE_SIZE){
    if(data->space[data->pth_i+itter*data->pth_number]!=' '){
      ++itter;
      continue;
    }
    if(!endgame(data, itter)){
      if(!defence(data, itter)){
        priority(data, itter);
      }
    }
    ++itter;
  }
  return NULL;
}
//------------------------------------------------------------------------------
// ready =======================================================================
bool init_game(char* space, bool* player){
  pthread_t thread_id;
  for(short int i=0; i<SPASE_SIZE; i++){
    for(short int j=0; j<SPASE_SIZE; j++){
      space[i*SPASE_SIZE+j]=' ';
      if(space[i*SPASE_SIZE+j]!=' '&&space[i*SPASE_SIZE+j]!='x'&&space[i*SPASE_SIZE+j]!='o'){
        printf("Wrong space!\n");
        return 1;
      }
    }
    if(space[i*SPASE_SIZE]==' '){
      continue;
    }
    *player=0;
    for(short int j=1; j<SPASE_SIZE; j++){
      if(space[i*SPASE_SIZE+j]!=space[i*SPASE_SIZE]){
        *player=1;
        break;
      }
    }
    if(!(*player)){
      printf("Its a gameover position! Exit!\n");
      return 1;
    }
  }
  printf("Enter who play now:");
  char c = getchar();
  for(short int i=0; i<SPASE_SIZE; i++){
    if (space[i]==' '){
      continue;
    }
    *player=0;
    for(short int j=1; j<SPASE_SIZE; j++){
      if(space[i+j*SPASE_SIZE]!=space[i]){
        *player=1;
        break;
      }
    }
      if(!(*player)){
        if(c==space[i]){
          printf("Its gameover! You win!\n");
          return 1;
        } else{
          printf("Its gameover! You louse!\n");
          return 1;
        }
      }
  }
  *player=0;
  for(short int i=0; i<SPASE_SIZE*SPASE_SIZE; i+=SPASE_SIZE+1){
    if (space[i]==' '){
      *player=1;
      break;
    } else if(space[i]!=space[0]){
      *player=1;
      break;
    }
  }
  if(!(*player)){
    if(c==space[0]){
      printf("Its gameover! You win!\n");
      return 1;
    } else{
      printf("Its gameover! You louse!\n");
      return 1;
    }
  }
  *player=0;
  for(short int i=0; i<SPASE_SIZE; i++){
    if(space[SPASE_SIZE-1+i*(SPASE_SIZE-1)]==' '){
      *player=1;
      break;
  } else if(space[SPASE_SIZE-1+i*(SPASE_SIZE-1)]!=space[SPASE_SIZE-1]){
      *player = 1;
      break;
    }
  }
  *player = (c=='x');
  return 0;
}
//------------------------------------------------------------------------------
//begin ========================================================================
int main(int argc, char const *argv[]) {
  bool player;
  char* space=(char*)malloc(SPASE_SIZE*SPASE_SIZE);
  short int command;
  if(argc==2){
    command=atoi(argv[1])-1;
    if(command<1){
      printf("Very small number!\n");
      return 1;
    }
  } else if(argc<2){
    command=SPASE_SIZE*SPASE_SIZE;
  }
  else{
    printf("Too math args!\n");
    return 2;
  }
  if(init_game(space, &player)){
    return 1;
  }
	short int index;

	if(!player){
		for (short int i = 0;  i < SPASE_SIZE; i++) {
			for(short int j= 0; j<SPASE_SIZE; j++){
				printf("|%d", i*SPASE_SIZE + j);
			}
			printf("|\n");
		}
		printf("Enter number of check:");
		scanf("%hd", &index);
			space[index]='x';
	}

	while(1){
  pthdata* massiv=(pthdata*)malloc(sizeof(pthdata)*command);
  pthread_t *thread_id = (pthread_t *) malloc(command * sizeof(pthread_t));
  for(short int i=0; i<command; i++){
    massiv[i].pth_number=command;
    massiv[i].pth_i=i;
    massiv[i].player=player;
    massiv[i].space=space;
    pthread_create(&thread_id[i], NULL, pth_function, (void*)&massiv[i]);
  }
  for(short int i=0; i<command; i++){
    pthread_join(thread_id[i], NULL);
  }
  /*for(short int i=0; i<command; i++){
    for(short int j=0; j<(SPASE_SIZE*SPASE_SIZE)/massiv[i].pth_number + (massiv[i].pth_i < SPASE_SIZE*SPASE_SIZE%massiv[i].pth_number); j++){
      printf("priority: %d\n", massiv[i].priority[j]);
    }
  }*/
  short int max=0;
  for(short int i=0; i<command; i++){
    for(short int j=0; j<(SPASE_SIZE*SPASE_SIZE)/massiv[i].pth_number + (massiv[i].pth_i < SPASE_SIZE*SPASE_SIZE%massiv[i].pth_number); j++){
      if(massiv[i].priority[j] > max){
        max = massiv[i].priority[j];
      }
    }
  }
	if(max==0){
		printf("I haven't idea! Goodbay!\n");
		return 0;
	}
  for(short int i=0; i<command; i++){
		index = i;
    for(short int j=0; j<(SPASE_SIZE*SPASE_SIZE)/massiv[i].pth_number + (massiv[i].pth_i < SPASE_SIZE*SPASE_SIZE%massiv[i].pth_number); j++){
      if(massiv[i].priority[j]==max){
        if(player){
          space[index]='x';
        } else{
          space[index]='o';
        }
				i = -1;
				break;
      }
			index+=command;
		}
		if(i == -1){
			break;
		}
	}
	for (short int i = 0;  i < SPASE_SIZE; i++) {
		for(short int j= 0; j<SPASE_SIZE; j++){
			printf("|%c", space[i*SPASE_SIZE + j]);
		}
		printf("|\n");
	}
	printf("----------------------------------------------------\n");
	for (short int i = 0;  i < SPASE_SIZE; i++) {
		for(short int j= 0; j<SPASE_SIZE; j++){
			printf("|%d", i*SPASE_SIZE + j);
		}
		printf("|\n");
	}
	printf("Enter number of check:");
	scanf("%hd", &index);
	if(player){
		space[index]='o';
	} else{
		space[index]='x';
	}
}
  return 0;
}
