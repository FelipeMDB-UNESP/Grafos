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
#include <time.h>
#include <limits.h>

#pragma region vetores
int* criar_vetor(int n) {
    int* vetor = (int*)malloc(sizeof(int) * n);
    return vetor;
}

void liberar_vetor(int* vetor) {
    free(vetor);
}
#pragma endregion vetores

#pragma region filas
typedef struct dado {
    int dado;
    struct dado* proximo;
} DADO;

typedef struct {
    DADO* entrada;
    DADO* saida;
} Fila;

typedef Fila* p_fila;

p_fila criar_fila() {
    p_fila f = malloc(sizeof(Fila));
    f->entrada = NULL;
    f->saida = NULL;
    return f;
}

int fila_vazia(p_fila f){
    return (f->saida == NULL);
}

void esvaziar_fila(p_fila f){
    DADO* aux;

    while(!fila_vazia(f)) {
        aux = f->saida;
        f->saida = f->saida->proximo;
        free(aux);
    }
    f->entrada = NULL;
}

void liberar_fila(p_fila f){
    DADO* aux;

    while(!fila_vazia(f)) {
        aux = f->saida;
        f->saida = f->saida->proximo;
        free(aux);
    }
    free(f);
}

void enfileirar(p_fila f, int k) {
    DADO* aux = malloc(sizeof(DADO));
    aux->dado = k;
    aux->proximo = NULL;

    if (!fila_vazia(f)) {
        f->entrada->proximo = aux;
        fila->entrada = aux;
    } else {
        fila->entrada = aux;
        fila->saida = aux;
    }
}

int desenfileirar(p_fila f) {
    DADO* aux;
    int i;

    if (!fila_vazia(f)) {
        aux = f->saida;
        if (f->entrada != f->saida) {
            f->saida = f->saida->proximo;
        } else {
            f->entrada = NULL;
            f->saida = NULL;
        }
        i = aux->dado;
        free(aux);
        return i;
    }
    return INT_MIN;
}
#pragma endregion filas

#pragma region matrizes
typedef struct matricial {
    int n;     //linhas
    int **matriz; //ponteiro para matriz
}Matriz;

Matriz* inicializar_matriz(int n){

    Matriz* matricial = (Matriz*) malloc(sizeof(Matriz));
    int** matriz_adjascencia = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matriz_adjascencia[i] = (int*)malloc(n * sizeof(int));
    }
    matricial->n = n;
    matricial->matriz = matriz_adjascencia;
    return matricial;
}

void liberar_matriz(Matriz* matricial){

    for (int i = 0; i < matricial->n; i++) {
        free(matricial->matriz[i]);
    }
    free(matricial->matriz);
    free(matricial->n);
    free(matricial);
}
#pragma endregion matrizes

#pragma region grafo_aleatorio
int probabilidade() {
    int prob;
    printf("Qual a probabilidade de cada aresta? (inteiros entre 1 e 100)\n");
    fflush(stdin);
    scanf(" %d", &prob);
    return prob;
}

void gerar_grafo(Matriz* matricial, bool orientado) {
    int prob = probabilidade();

    srand(time(NULL));
    if (!orientado) {   //garante espelhamento 
        for (int i = 0; i < matricial->n; i++) {
            for (int j = i; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    matricial->matriz[i][j] = (rand() % 100 < prob) ? 1  * (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
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
                    matricial->matriz[i][j] = (rand() % 100 < prob) ? 1  * (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
                } else {
                    matricial->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    }
}

#pragma endregion

int* busca_em_largura(Matriz* grafo, int k) {
    int i, j;
    int* pai = criar_vetor(grafo->n);
    int* visitado = criar_vetor(grafo->n);
    p_fila f = criar_fila();

    for (i=0; i < grafo->n; i++) {
        pai[i]= -1;
        visitado[i] = 0;
    }
    
    enfileirar(f,k);
    pai[k] = k;
    visitado[k] = 1;

    while (!fila_vazia(f)) {
        for (i = desenfileirar(f), j = 0; j < grafo->n; j++) {
            if (grafo->matriz[i][j] && !visitado[j]) {
                visitado[j] = 1;
                pai[j] = i;
                enfileirar(f, j);
            }
        }
    }

    liberar_fila(f);
    liberar_vetor(visitado);
    return pai;
}

int main() {

}