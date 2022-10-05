#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

/*Variaveis globais*/
int first_condition = 0;
pthread_mutex_t first_condition_mutex;
pthread_cond_t first_condition_cond;

int second_condition = 0;
pthread_mutex_t second_condition_mutex;
pthread_cond_t second_condition_cond;

void *thread1 (void *t){
    pthread_mutex_lock(&second_condition_mutex);
    if(second_condition < 2){
        pthread_cond_wait(&second_condition_cond, &second_condition_mutex);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&second_condition_mutex);
    pthread_exit(NULL);
}

void *thread2 (void *t){
    pthread_mutex_lock(&first_condition_mutex);
    if(first_condition < 1){
        pthread_cond_wait(&first_condition_cond, &first_condition_mutex);
    }
    second_condition++;
    if(second_condition == 2){
        pthread_cond_signal(&second_condition_cond);
    }
    printf("Fique a vontade.\n");
    pthread_mutex_unlock(&first_condition_mutex);
    pthread_exit(NULL);
}

void *thread3 (void *t){
    pthread_mutex_lock(&first_condition_mutex);
    if(first_condition < 1){
        pthread_cond_wait(&first_condition_cond, &first_condition_mutex);
    }
    second_condition++;
    if(second_condition == 2){
        pthread_cond_signal(&second_condition_cond);
    }
    printf("Sente-se por favor.\n");
    pthread_mutex_unlock(&first_condition_mutex);
    pthread_exit(NULL);
}

void *thread4 (void *t){
    printf("Seja bem-vindo!\n");

    pthread_mutex_lock(&first_condition_mutex);
    first_condition++;
    if(first_condition == 1){
        pthread_cond_broadcast(&first_condition_cond);
    }
    pthread_mutex_unlock(&first_condition_mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int i;
    pthread_t threads[NTHREADS];

    /* Inicilaiza os mutex (lock de exclusao mutua) e as variaveis de condicao */
    pthread_mutex_init(&first_condition_mutex, NULL);
    pthread_mutex_init(&second_condition_mutex, NULL);
    pthread_cond_init(&first_condition_cond, NULL);
    pthread_cond_init(&second_condition_cond, NULL);

    /* Cria as threads */
    pthread_create(&threads[3], NULL, thread1, NULL);
    pthread_create(&threads[2], NULL, thread2, NULL);
    pthread_create(&threads[0], NULL, thread3, NULL);
    pthread_create(&threads[1], NULL, thread4, NULL);

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&first_condition_mutex);
    pthread_mutex_destroy(&second_condition_mutex);
    pthread_cond_destroy(&first_condition_cond);
    pthread_cond_destroy(&second_condition_cond);
    return 0;
}