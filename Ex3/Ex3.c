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
        f->entrada = aux;
    } else {
        f->entrada = aux;
        f->saida = aux;
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

    int n;          //linhas
    int **matriz; //ponteiro para matriz
}Matriz;

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

void liberar_matriz(Matriz* matricial){

    for (int i = 0; i < matricial->n; i++) {
        free(matricial->matriz[i]);
    }
    free(matricial->matriz);
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

//função que gera um grafo com arestas aleatorias
void gerar_grafo(Matriz* matricial, bool orientado, int probabilidade) {
    
    
    if (!orientado) {   //garante espelhamento 
        for (int i = 0; i < matricial->n; i++) {
            for (int j = i; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    srand(time(NULL));
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

#pragma endregion

int* busca_em_largura(Matriz* grafo, int origem) {

    int i, j;
    int* pai = criar_vetor(grafo->n);
    int* visitado = criar_vetor(grafo->n);
    p_fila f = criar_fila();

    for (i=0; i < grafo->n; i++) {
        pai[i]= -1;
        visitado[i] = 0;
    }
    
    enfileirar(f,origem);
    pai[origem] = origem;
    visitado[origem] = 1;

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

    int qtd_vertices, origem, direcionado, prob;
    printf("Qual a quantidade de vertices?\n");
    scanf(" %d", &qtd_vertices);

    prob = probabilidade();

    printf("Qual o vertice de origem?\n");
    scanf(" %d", &origem);

    // Verifica se a origem está dentro do intervalo válido
    if (origem < 0 || origem >= qtd_vertices) {

        printf("Origem invalida.\n");
        return 1;
    }

    //verifica se eh direcionado ou nao
    do {

        printf("\nSelecione:\n0 - Grafo NAO direcionado\n1 - Grafo direcionado\n");
        scanf(" %d", &direcionado);

        if (direcionado != 0 && direcionado != 1) {
            printf("\nOPCAO INVALIDA!!!\n");
        }
    } while (direcionado != 0 && direcionado != 1);


    Matriz* grafo = inicializar_matriz(qtd_vertices);


    int aux;
    do {

        printf("\nDeseja um grafo aleatorio? [1=SIM/0=NAO]\n");
        scanf(" %d", &aux);

        if (aux != 0 && aux != 1) {
            printf("\nOPCAO INVALIDA!!!\n");
        }
    } while (aux != 0 && aux != 1);

    if (aux) {
        
        gerar_grafo(grafo, direcionado, prob);
    } else {

        // Preenche a matriz de distâncias
        printf("Informe as distancias entre os vertices (use -1 para indicar ausencia de aresta):\n");
        for (int i = 0; i < grafo->n; i++) {

            for (int j = 0; j < grafo->n; j++) {

                if (i == j){

                    grafo->matriz[i][j] = 0;
                }
                else if(direcionado) {

                    printf("Distancia entre o vertice %d e %d: ", i, j);
                    scanf(" %d", &grafo->matriz[i][j]);
                }
                else if (i < j && !direcionado){

                    printf("Distancia entre o vertice %d e %d: ", i, j);
                    scanf(" %d", &grafo->matriz[i][j]);
                    grafo->matriz[j][i] = grafo->matriz[i][j];
                }

                if (grafo->matriz[i][j] == -1) {

                    grafo->matriz[i][j] = 0; // Considerando 0 para ausência de aresta
                }
            }
        }
    }

    // Realiza a busca em largura
    int* resultado = busca_em_largura(grafo, origem);

    // Exibe o vetor de pais retornado pela busca em largura
    printf("\nVetor de pais na busca em largura a partir do vertice %d:\n", origem);
    for (int i = 0; i < grafo->n; i++) {
        printf("Vertice %d: Pai = %d\n", i, resultado[i]);
    }

    // Libera a memória utilizada
    liberar_vetor(resultado);
    liberar_matriz(grafo);

    return 0;
}
