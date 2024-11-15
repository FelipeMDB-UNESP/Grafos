/*
Por:
André Luis Dias Nogueira
Felipe Melchior de Britto
Ryan Hideki Tadeo Guimarães
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#pragma region matrizes

/*
Estrutura genérica de matriz. Criada com o intuito de construir um grafo.

Objetos da estrutura:
int n -> linhas.
int ** matriz -> ponteiro para matriz.
*/
typedef struct matricial {

    int n;          //linhas
    int **matriz; //ponteiro para matriz
}Matriz;


/*
Função que inicializa uma matriz para grafos.

Parâmetros:
int qtd_vertices -> quantidade de vértices desejados.

Retorno 
ponteiro para a matriz inicializada.
*/
Matriz* inicializar_matriz(int qtd_vertices){

    Matriz* matricial = (Matriz*) malloc(sizeof(Matriz));
    int** matriz_adjascencia = (int**) malloc(qtd_vertices * sizeof(int*));
    for (int i = 0; i < qtd_vertices; i++) {
        matriz_adjascencia[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }
    matricial->n = qtd_vertices;
    matricial->matriz = matriz_adjascencia;
    return matricial;
}

/*
Função que libera o espaço dinamicamente alocado para uma matriz, incluindo todos os dados armazenados nela.

Parâmetros:
Matriz* matricial -> ponteiro para a matriz a ser liberada.
*/
void liberar_matriz(Matriz* matricial){

    for (int i = 0; i < matricial->n; i++) {
        free(matricial->matriz[i]);
    }
    free(matricial->matriz);
    free(matricial);
}
#pragma endregion matrizes

#pragma region grafo_aleatorio

typedef Matriz* Grafo;

/*
Função para gerar um grafo com arestas aleatórias.

Parâmetros:
Matriz* matricial -> ponteiro de matriz para se gerar as arestas.
bool orientado -> booleano que informa se a matriz é orientada ou não.
int probabilidade -> probabilidade de cada aresta.
*/
void gerar_grafo(Grafo grafo, bool orientado, float probabilidade) {
    
    int porcentagem = (int) (100*probabilidade);
    srand(time(NULL));

    if (!orientado) {   //garante espelhamento 
        for (int i = 0; i < grafo->n; i++) {
            for (int j = i; j < grafo->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    grafo->matriz[i][j] = (rand() % 100 < porcentagem) ? (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
                    grafo->matriz[j][i] = grafo->matriz[i][j];
                    
                } else {
                    grafo->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    } else {
        for (int i = 0; i < grafo->n; i++) {
            for (int j = 0; j < grafo->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    grafo->matriz[i][j] = (rand() % 100 < porcentagem) ? (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
                } else {
                    grafo->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    }
}

#pragma endregion grafo_aleatorio