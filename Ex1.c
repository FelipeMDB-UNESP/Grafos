/*
Por:
André Luis Dias Nogueira
Felipe Melchior de Britto
Rafael Daiki Kaneko
Ryan Hideki Tadeo Guimarães
Vitor Marchini Rolisola
*/
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

struct grafo {
    int V; //vertices
    int A; //arestas
    int **adj; //ponteiro para matriz de adjacência
}Grafo;
typedef struct grafo *ponteiro_grafo;

void escreve_matriz_adjacencia(int qtd_vertices, int prob, bool orientado){
    int i, j;

    //alocacao de memoria
    int **matriz_adj = (int**)malloc(qtd_vertices * sizeof(int*));
    for (i = 0; i < qtd_vertices; i++) {
        matriz_adj[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }

        
    for (i = 0; i < qtd_vertices; i++) {
        for (j = 0; j < qtd_vertices; j++) {
            if (i != j) {                                                       //evitar ligacoes proprias
                matriz_adj[i][j] = (rand() % 100 < prob) ? 1 : 0;               //random de números entre 0 e 99 (resto da divisao por 100)
                if (!orientado) {                                               //garante espelhamento 
                    matriz_adj[j][i] = matriz_adj[i][j];
                }
            } else {
                matriz_adj[i][j] = 0;                                          //falso para quando for a diagonal principal       
            }
        }
    }

    FILE *arq = fopen("grafo.txt", "w");    //abre para escrita e cria arquivo se necessário
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    for (i = 0; i < qtd_vertices; i++) {
        for (j = 0; j < qtd_vertices; j++) {
            fprintf(arq, "%d ", matriz_adj[i][j]);
        }
        fprintf(arq, "\n");
    }
    fclose(arq);

    //libera memoria
    for (i = 0; i < qtd_vertices; i++) { 
        free(matriz_adj[i]);
    }
    free(matriz_adj);
}

int main() {

    bool orientado = false;
    int qtd_vertices, prob, orientacao;
    printf("Qual a quantidade de vertices?\n");
    scanf(" %d", &qtd_vertices);
    printf("Qual a probabilidade de cada aresta?\n");
    scanf(" %d", &prob);
    printf("Eh orientado?\nSIM = Digite [1]\nNAO = Digite [0]\n");
    scanf(" %d", &orientacao);

    //TODO: talvez melhorar essa lógica aqui da orientação
    if(orientacao == 1){
        orientado = true;
    }

    escreve_matriz_adjacencia(qtd_vertices, prob, orientado);

    return 0;
}
