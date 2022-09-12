#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

float *mat; //matriz de entrada
float *mat2; //matriz de entrada
float *saida; //matriz de saida

int main (int argc, char* argv[]){
    int dim; //dimensao da matriz de entrada
    double start, finish, elapsed;
    //leitura e avaliacao dos parametros de entrada
    if(argc < 2){
        printf("Digite: %s <dimensao da matriz> \n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);

    //alocacao de memoria para a estrutura da dados
    mat = (float *)malloc(sizeof(float) * dim * dim);
    if(mat == NULL){ printf("ERRO--malloc\n"); return 2; }
    mat2 = (float *)malloc(sizeof(float) * dim * dim);
    if(mat2 == NULL){ printf("ERRO--malloc\n"); return 3; }
    saida = (float *)malloc(sizeof(float) * dim * dim);
    if(saida == NULL){ printf("ERRO--malloc\n"); return 4; }

    //inicializacao da matriz de vetor
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            mat[i * dim + j] = 1;  //equivalente a mat[i][j]
            mat2[i * dim + j] = 1;
            saida[i * dim + j] = 0;
        }
    }


    // multiplicacao das matrizes
    GET_TIME(start);
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            for(int k = 0; k < dim; k++){
                saida[(i*dim)+j] += mat[(i*dim)+k] * mat2[(k*dim)+j];
            }
        }
    }
    GET_TIME(finish);

    //exibicao do resultado
    // puts("Matriz de entrada");
    // for(int i = 0; i < dim; i++){
    //     for(int j = 0; j < dim; j++){
    //         printf("%.1f ", mat[i * dim + j]);
    //     }    
    //     puts("");
    // }
    // puts("Matriz de entrada 2");
    // for(int i = 0; i < dim; i++){
    //     for(int j = 0; j < dim; j++){
    //         printf("%.1f ", mat2[i * dim + j]);
    //     }    
    //     puts("");
    // }
    // puts("Matriz de saida");
    // for(int i = 0; i < dim; i++){
    //     for(int j = 0; j < dim; j++){
    //         printf("%.1f ", saida[i * dim + j]);
    //     }
    //     puts("");
    // }
    elapsed = finish - start;
    printf("tempo da multiplicacao: %e\n", elapsed);
    //liberacao da memoria
    free(mat);
    free(mat2);
    free(saida);
    
    return 0;
}