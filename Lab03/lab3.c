#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include "timer.h"

float *mat; //matriz de entrada
int linhas, colunas; //dimensoes da matriz
long long int tam; //qtde de elementos na matriz
float *mat2; //matriz de entrada
int linhas2, colunas2; //dimensoes da matriz
long long int tam2; //qtde de elementos na matriz
float *saida; //matriz de saida
int nThreads;

void * tarefa (void * arg){
    int ident = * (int *) arg;
    int start = (linhas/nThreads) * (ident-1);
    int end = (linhas/nThreads) * ident;
    
    // int aux =0;

    // for(int i=args->id; i<args->dim; i+=nthreads){
    //   for(int j=0; j<args->dim; j++){ 
    //      Matriz_saida_Concorrente[i*args->dim +j]=0;
	// 	for(int k=0; k<args->dim; k++){
	// 	    aux+=mat1[i*args->dim +k]*mat2[k*args->dim+j];				
	// 	}
	// 	Matriz_saida_Concorrente[i*args->dim+j]=aux;
	// 	aux=0;
	// }
	// }

    for(int i = start; i < end; i++){
        for(int j = 0; j < colunas2; j++){
            for(int k = 0; k < colunas; k++){
                saida[i*colunas2+j]+=mat[i*colunas +k]*mat2[k*colunas2+j];
                // saida[(i*colunas2)+j] += mat[(i*colunas)+k] * mat2[(k*colunas2)+j];
            }
            // saida[i*colunas2+j] = aux;
        }
    }

    pthread_exit(NULL);
}

int main (int argc, char* argv[]){
    size_t ret; //retorno da funcao de escrita no arquivo de saida
    double start, finish, elapsed;
    FILE * Arquivo; //descritor do arquivo de saida
    //leitura e avaliacao dos parametros de entrada
    if(argc < 5){
        printf("Digite: %s <nome do primeiro arquivo de entrada> <nome do segundo arquivo de entrada> <nome do arquivo de saida> <numero de threads para usar>\n", argv[0]);
        return 1;
    }

    GET_TIME(start);

    // nThreads = (int)argv[4];
    nThreads = 4;

    //abre o arquivo para leitura binaria
    Arquivo = fopen(argv[1], "rb");
    if(!Arquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    //le as dimensoes da primeira matriz
    ret = fread(&linhas, sizeof(int), 1, Arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fread(&colunas, sizeof(int), 1, Arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    tam = linhas * colunas; //calcula a qtde de elementos da matriz

    mat = (float *)malloc(sizeof(float) * linhas * colunas);
        if(mat == NULL){ printf("ERRO--malloc\n"); return 2; }

        //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(mat, sizeof(float), tam, Arquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 4;
    }

    //abre o arquivo para leitura binaria
    Arquivo = fopen(argv[2], "rb");
    if(!Arquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    //le as dimensoes da primeira matriz
    ret = fread(&linhas2, sizeof(int), 1, Arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    ret = fread(&colunas2, sizeof(int), 1, Arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }
    tam2 = linhas2 * colunas2; //calcula a qtde de elementos da matriz

    mat2 = (float *)malloc(sizeof(float) * linhas2 * colunas2);
        if(mat2 == NULL){ printf("ERRO--malloc\n"); return 2; }

        //carrega a matriz de elementos do tipo float do arquivo
    ret = fread(mat2, sizeof(float), tam2, Arquivo);
    if(ret < tam2) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 4;
    }

    //alocacao de memoria para a estrutura da dados
    saida = (float *)malloc(sizeof(float) * linhas * colunas2);
    if(saida == NULL){ printf("ERRO--malloc\n"); return 4; }

    GET_TIME(finish);
    elapsed = finish - start;
    printf("tempo da preparação: %lf\n", elapsed);

    // multiplicacao das matrizes
    GET_TIME(start);

    pthread_t tid[nThreads];
    int ident[nThreads];
    for(int i = 0; i < nThreads; i++){
        ident[i] = i+1;
        if(pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i]))
            printf("ERRO -- pthread_create\n");
    }

    for(int thread=0; thread<nThreads; thread++){
        if(pthread_join(tid[thread], NULL)){
            printf("--ERRO: pthread_join()"); 
            exit(-1);
        }
    }

    // for(int i = 0; i < linhas; i++){
    //     for(int j = 0; j < colunas2; j++){
    //         for(int k = 0; k < colunas; k++){
    //             saida[(i*colunas2)+j] += mat[(i*colunas)+k] * mat2[(k*colunas2)+j];
    //         }
    //     }
    // }

    GET_TIME(finish);
    elapsed = finish - start;
    printf("tempo da multiplicacao: %lf\n", elapsed);


    GET_TIME(start);

    //escreve a matriz no arquivo
    //abre o arquivo para escrita binaria
    Arquivo = fopen(argv[2], "wb");
    if(!Arquivo) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }
    //escreve numero de linhas e de colunas
    ret = fwrite(&linhas, sizeof(int), 1, Arquivo);
    //escreve os elementos da matriz
    ret = fwrite(&colunas2, sizeof(int), 1, Arquivo);
    ret = fwrite(saida, sizeof(float), tam, Arquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de escrita no  arquivo\n");
        return 5;
    }

    //liberacao da memoria
    fclose(Arquivo);
    free(mat);
    free(mat2);
    free(saida);

    GET_TIME(finish);
    elapsed = finish - start;
    printf("tempo da escrita: %lf\n", elapsed);
    
    return 0;
}