#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

typedef struct matricial {
    int n;     //linhas
    int **matriz; //ponteiro para matriz
}Matriz;

Matriz* inicializar_matriz(int n){

    Matriz* matricial = (Matriz*) malloc(sizeof(Matriz));
    int** matriz_distancias = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matriz_distancias[i] = (int*)malloc(n * sizeof(int));
    }
    matricial->n = n;
    matricial->matriz = matriz_distancias;
    return matricial;
}

void liberar_matriz(Matriz* matricial){

    for (int i = 0; i < qtd_vertices; i++) {
        free(matricial->matriz[i]);
    }
    free(matricial->matriz);
    free(matricial->n);
    free(matricial);
}

void gerar_grafo(Matriz* matricial, bool orientado, int prob) {
    srand(time(NULL));
    if (!orientado) {   //garante espelhamento 
        for (i = 0; i < matricial->n; i++) {
            for (j = i; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    matricial->matriz[i][j] = (rand() % 100 < prob) ? 1  * (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
                    matricial->matriz[j][i] = matricial->matriz[i][j];
                    
                } else {
                    matricial->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    } else {
        for (i = 0; i < matricial->n; i++) {
            for (j = 0; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    matricial->matriz[i][j] = (rand() % 100 < prob) ? 1  * (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
                } else {
                    matricial->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    }
}

int* criar_vetor(int n) {
    int* vetor = (int*)malloc(sizeof(int) * n);
    return vetor;
}

void liberar_vetor(int* vetor) {
    free(vetor);
}