#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

struct grafo {
    int V; //vertices
    int A; //arestas
    int **adj; //ponteiro para matriz de adjacência
}Grafo;
typedef struct grafo *ponteiro_grafo;

int main(){

    FILE *arq;
    arq = fopen("grafo", "w");



    fclose(arq);
}

void escreve_matriz_adjacencia(int qtd_vertices, int prob, bool orientado){

    (int*) matriz_adj = (int*)malloc(sizeof(int)*qtd_vertices);
    

    if(orientado){

    }else{

    }
}