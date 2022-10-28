#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NELEMENTOSBUFFER 5

// Variaveis globais
int Buffer[NELEMENTOSBUFFER];
int NThreadsProd = 0;
int NThreadsCons = 0;
int in = 0;
int out = 0;
int count = 0;
sem_t slotCheio, slotVazio;
sem_t mutexProd, mutexCons;

void printArray(){
  for(int i = 0; i<NELEMENTOSBUFFER; i++){
    printf("%d ", Buffer[i]);
  }
  printf("\n");
}

void Insere (int item) {
  sem_wait(&slotVazio);
  sem_wait(&mutexProd);
  Buffer[in] = item;
  in = (in + 1) % NELEMENTOSBUFFER;
  count++;
  printf("count : %d\n", count);
  printArray();
  if(count == NELEMENTOSBUFFER){
    sem_post(&slotCheio);
  }
  sem_post(&mutexProd);
}

int Retira (void) {
  int item;
  sem_wait(&slotCheio);
  sem_wait(&mutexCons);
  item = Buffer[out];
  Buffer[out] = 0;
  out = (out + 1) % NELEMENTOSBUFFER;
  count--;
  printf("count : %d\n", count);
  printArray();
  sem_post(&mutexCons);
  sem_post(&slotVazio);
  return item;
}

void *produtor(void * arg) {
  int elemento = 3;
  while(1) {
    printf("aguardando entrar produtor\n");
    Insere(elemento);
    printf("saindo produtor\n");
    sleep(1);
  }
  pthread_exit(NULL);
}

void *consumidor(void * arg) {
    int elemento;
    while(1) {
      printf("aguardando entrar consumidor\n");
      elemento = Retira();
      printf("saindo consumidor\n");
      sleep(1);
    }
    pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("Digite: %s <quantidade de threads de produtor> <quantidade de threads de consumidor>\n", argv[0]);
    return 1;
  }

  NThreadsProd = atoi(argv[1]);
  NThreadsCons = atoi(argv[2]);
  int NThreads = NThreadsProd+NThreadsCons;
  pthread_t tid[NThreads];
  int *id[NThreads], t;

  //inicia os semaforos
  sem_init(&mutexCons, 0, 1);
  sem_init(&mutexProd, 0, 1);
  sem_init(&slotCheio, 0, 0);
  sem_init(&slotVazio, 0, NELEMENTOSBUFFER);

  for (t=0; t<NThreads; t++) {
    if ((id[t] = malloc(sizeof(int))) == NULL) {
      pthread_exit(NULL); return 1;
    }
    *id[t] = t+1;
  }

  //cria as tres threads
  for (int i = 0; i < NThreads; i++){
    if(i < NThreadsProd){
      if (pthread_create(&tid[i], NULL, produtor, (void *)id[i])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
    }
    else{
      if (pthread_create(&tid[i+NThreadsProd], NULL, consumidor, (void *)id[i+NThreadsProd])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
    }
  }
  pthread_exit(NULL);
}