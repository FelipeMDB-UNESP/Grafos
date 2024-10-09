/*
Por:
André Luis Dias Nogueira
Felipe Melchior de Britto
Rafael Daiki Kaneko
Ryan Hideki Tadeo Guimarães
Vitor Marchini Rolisola
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
/*
Função para gerar um grafo com arestas aleatórias.

Parâmetros:
Matriz* matricial -> ponteiro de matriz para se gerar as arestas.
bool orientado -> booleano que informa se a matriz é orientada ou não.
int probabilidade -> probabilidade de cada aresta.
*/
void gerar_grafo(Matriz* matricial, bool orientado, int probabilidade) {
    
    srand(time(NULL));
    
    if (!orientado) {   //garante espelhamento 
        for (int i = 0; i < matricial->n; i++) {
            for (int j = i; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    matricial->matriz[i][j] = (rand() % 100 < probabilidade) ? (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
                    matricial->matriz[j][i] = matricial->matriz[i][j];
                    
                } else {
                    matricial->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    } else {
        for (int i = 0; i < matricial->n; i++) {
            for (int j = 0; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    matricial->matriz[i][j] = (rand() % 100 < probabilidade) ? 1  * (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
                } else {
                    matricial->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    }
}

#pragma endregion grafo_aleatorio

#pragma region lista

typedef struct lista {
    int item;
    struct lista *prox;
} LISTA;

typedef LISTA* p_lista;

p_lista criar_lista() {

    p_lista list = NULL;
    return list;
}

void inserir_lista(p_lista* list, int item) {

    p_lista aux = (p_lista) malloc(sizeof(LISTA));
    aux->item = item;
    aux->prox = *list;
    *list = aux;
}

int retirar_lista(p_lista* list) {

    int item;
    p_lista aux;

    item = (*list)->item;
    aux = (*list)->prox;

    free(*list);
    *list = aux;

    return item;
}

void liberar_lista(p_lista* list) {

    p_lista aux;

    while (*list !=NULL) {
        aux = *list;
        *list = aux->prox;
        free(aux);
    }
}
#pragma endregion lista

#pragma region clique
bool conectado(Matriz* grafo, p_lista list, int vertice) {

    while (list != NULL) {

        if (!(grafo[vertice][list->item] || grafo[list->item][vertice]))
            return false;

        list = list->prox;
    }
    return true;
}

p_lista clique_maximal(Matriz* grafo, int vertice) {

    p_lista list = criar_lista();
    inserir_lista(&list, vertice);

    for (int item = 0; item < grafo->n; item++) {

        if (item != vertice && conectado(grafo, list, item))
            inserir_lista(&list, item);

    }
    return list;
}

#pragma endregion clique

int main() {

}