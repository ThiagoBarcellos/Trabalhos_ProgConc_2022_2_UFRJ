#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

int numeroGlobal = 0;
int leit = 0;
int esc = 0;
int escLeit = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_esc;

void EntraLeitura() {
    pthread_mutex_lock(&mutex);
    while(esc > 0 || escLeit > 0) {
        pthread_cond_wait(&cond_leit, &mutex);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

void SaiLeitura() {
    pthread_mutex_lock(&mutex);
    leit--;
    if(leit==0) pthread_cond_signal(&cond_esc);
    pthread_mutex_unlock(&mutex);
}

void EntraEscrita() {
    pthread_mutex_lock(&mutex);
    while(leit > 0 || escLeit > 0) {
        pthread_cond_wait(&cond_esc, &mutex);
    }
    esc++;
    pthread_mutex_unlock(&mutex);
}

void SaiEscrita() {
    pthread_mutex_lock(&mutex);
    esc--;
    pthread_cond_signal(&cond_esc);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

void EntraEscritaELeitura() {
    pthread_mutex_lock(&mutex);
    while(leit > 0 || esc > 0) {
        pthread_cond_wait(&cond_esc, &mutex);
    }
    escLeit++;
    pthread_mutex_unlock(&mutex);
}

void SaiEscritaELeitura() {
    pthread_mutex_lock(&mutex);
    escLeit--;
    pthread_cond_signal(&cond_esc);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

void *T1 (void *t){
    while(1){
        EntraEscrita();
        numeroGlobal++;
        SaiEscrita();
        sleep(1);
    }
    pthread_exit(NULL);
}

void *T2 (void *t){
    while(1){
        EntraLeitura();
        numeroGlobal % 2 == 0 ? printf("[2] O número %d é par\n", numeroGlobal) : printf("[2] O número %d é impar\n", numeroGlobal);
        SaiLeitura();
        sleep(1);
    }
    pthread_exit(NULL);
}

void *T3 (void *t){
    int ident = * (int *) t;
    while(1){
        int inicio = -100000;
        int fim = 100000;
        EntraEscritaELeitura();
        printf("[3] %d\n", numeroGlobal);
        while(inicio < fim) { inicio ++; }
        numeroGlobal = ident;
        SaiEscritaELeitura();
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int quantLeit = 0;
    int quantEsc = 0;
    int quantEscLeit = 0;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_esc, NULL);
    //leitura e avaliacao dos parametros de entrada
    if(argc < 4){
        printf("Digite: %s <quantidade de threads de leitura> <quantidade de threads de escrita> <nome do arquivo de saida> <numero de threads para usar>\n", argv[0]);
        return 1;
    }

    quantLeit = atoi(argv[1]);
    quantEsc = atoi(argv[2]);
    quantEscLeit = atoi(argv[3]);
    int quantThreads = quantEsc+quantLeit+quantEscLeit;
    printf("%d\n", quantThreads);
    pthread_t tid[quantThreads];
    int id[quantThreads]; 

    //cria as threads leitoras
    for(int i=0; i<quantLeit; i++) {
        id[i] = i+1;
        if(pthread_create(&tid[i], NULL, T2, (void *) &id[i])) exit(-1);
    } 
  
    //cria as threads escritoras
    for(int i=0; i<quantEsc; i++) {
        id[i+quantLeit] = i+1;
        if(pthread_create(&tid[i+quantLeit], NULL, T1, (void *) &id[i+quantLeit])) exit(-1);
    } 

    //cria as threads escritorasLeitoras
    for(int i=0; i<quantEscLeit; i++) {
        id[i+quantLeit+quantEsc] = i+1;
        if(pthread_create(&tid[i+quantLeit+quantEsc], NULL, T3, (void *) &id[i+quantLeit+quantEsc])) exit(-1);
    } 

    for (int i = 0; i < quantThreads; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_leit);
    pthread_cond_destroy(&cond_esc);
}