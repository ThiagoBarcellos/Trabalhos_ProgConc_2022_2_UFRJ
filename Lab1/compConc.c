#include<stdio.h> 
#include <stdlib.h>
#include<pthread.h> 

#define NELEMENTS 10000
#define NTHREADS 2
#define THRESHOLD 0.0000001

float elements[NELEMENTS];
float copia[NELEMENTS];

int verifica (float vetor[]){
    for(int i = 0; i < NELEMENTS; i++){
        if(vetor[i] - copia[i] < THRESHOLD)
            return i;
    }
    return 0;
}

void * tarefa (void * arg) { 
   int ident = * (int *) arg; 
   int start = (NELEMENTS / NTHREADS) * (ident-1);
   int end = (NELEMENTS / NTHREADS) * ident;
   for(int i = start; i < end; i++){
        elements[i] *= 1.1;
   }
   pthread_exit(NULL); 
} 
 
int main(void) {
    for(int i = 0; i < NELEMENTS; i++){
        elements[i] = i+1;
        copia[i] = i+1;
    }

    pthread_t tid[NTHREADS];
    int ident[NTHREADS]; 

    for(int i = 0; i < NTHREADS; i++){
        ident[i] = i +1;
        if (pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i]))  
            printf("ERRO -- pthread_create\n"); 
    }

    for(int thread=0; thread<NTHREADS; thread++){
        if(pthread_join(tid[thread], NULL)){
            printf("--ERRO: pthread_join()"); 
            exit(-1);
        }
    }

    int response = verifica(elements);
    if(!response){
        printf("O vetor foi aumentado com sucesso! \n");
    }else{
        printf("A posição %d não foi alterada com sucesso!", response);
    }

    return 0; 
}