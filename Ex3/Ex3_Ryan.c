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
#include <time.h>

struct grafo {
    int V;     //vertices
    int A;     //arestas
    int **adj; //ponteiro para matriz de adjacência
}Grafo;
typedef struct grafo *ponteiro_grafo;

// Estruturas de Fila
typedef struct noh
{
    int vertice;
    struct Noh *prox_noh;
} Noh;

typedef struct fila
{
    Noh *inicio;
    Noh *final;
}Fila;

// Funções de Fila
void inicializaFila(Fila *fila) {
    fila->inicio = NULL;
    fila->final = NULL;
}

void insereFila(Fila *fila, int valor){
    Noh *aux;
    aux = malloc(sizeof(Noh));
    aux->vertice = valor;
    aux->prox_noh = NULL;

    if (fila->inicio == NULL){ // Lista vazia
        fila->inicio = aux;
        fila->final = aux;
    }
    else{
        fila->final->prox_noh = aux;
        fila->final = aux;
    }
}

int filaVazia(Fila *fila){
    return (fila->inicio == NULL);
}

int removerFila(Fila *fila) {
    if (filaVazia(fila)) {
        return -1; // Retorna valor inválido se a fila estiver vazia
    }
    Noh *aux = fila->inicio;
    int removido = aux->vertice;
    fila->inicio = fila->inicio->prox_noh;
    if (fila->inicio == NULL) {
        fila->final = NULL;
    }
    free(aux);
    return removido;
}

void liberaFila(Fila *fila){
    Noh *aux;
    while (fila->inicio != NULL){
        aux = fila->inicio;
        fila->inicio = fila->inicio->prox_noh;
        free(aux);    
    }
    fila->final = NULL;
}
 
// s -> noh raiz
void busca_largura (int grau, int** matriz_adj, int s){
    if (s < 0 || s > grau-1){ // Tratamento de erros
        printf("\nVertice fora dos limites! (nao esta entre o vertice 0 e %d)\n\n", grau-1);
        return;
    }
    int v, w;
    int *pai = malloc(grau * sizeof(int)); 
    int *visitado = malloc(grau * sizeof(int));
    Fila f;
    inicializaFila(&f);  
    // Inicializa as matrizes pai e visitado
    for (v = 0; v < grau; v++){
        pai[v] = -1;        // pai desconhecido
        visitado[v] = 0;    // Nenhum visitado ainda
    }
    insereFila(&f, s);
    pai[s] = s; // Define s como a raiz da arvore
    visitado[s] = 1;

    while (!filaVazia(&f))
    {
        v = removerFila(&f);
        for (w = 0; w < grau; w++){
            if (matriz_adj[v][w] && !visitado[w]){
                visitado[w] = 1;
                pai[w] = v;
                insereFila(&f, w);
            }
        }
    }
    liberaFila(&f);
    free(visitado);
    // Impressão dos resultados
    printf("Pais dos vertices:\n");
    for (int i = 0; i < grau; i++) {
        printf("Vertice %d: Pai %d\n", i, pai[i]);
    }
    free(pai);
}

int** escreve_matriz_adjacencia(int qtd_vertices, int prob, bool orientado, int s){
    int i, j;
    srand(time(NULL));
    //alocacao de memoria
    int **matriz_adj = (int**)malloc(qtd_vertices * sizeof(int*));
    for (i = 0; i < qtd_vertices; i++) {
        matriz_adj[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }
    for (i = 0; i < qtd_vertices; i++) {
        for (j = 0; j < qtd_vertices; j++) {
            if (i != j) {                                                       //evitar ligacoes proprias
                matriz_adj[i][j] = (rand() % 100 < prob) ? 1  * (rand() % 9 + 1) : 0;               //random de números entre 0 e 99 (resto da divisao por 100)
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
    fprintf(arq, "\n");
  
    fclose(arq);
    return matriz_adj;
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
    printf("Qual a probabilidade de cada aresta? (0 a 100%)\n");
    scanf(" %d", &prob);
    printf("Eh orientado?\nSIM = Digite [1]\nNAO = Digite [0]\n");
    scanf(" %d", &orientacao);

    if(orientacao == 1){
        orientado = true;
    }
    int raiz;
    
    printf("Qual vertice quer usar como raiz na Busca Binaria? (Escolha entre 0 e %d)\n", qtd_vertices-1);
    scanf(" %d", &raiz);
  
    int** grafo;
    grafo = escreve_matriz_adjacencia(qtd_vertices, prob, orientado, raiz);
    busca_largura(qtd_vertices, grafo, raiz);
 
    free(grafo);
    return 0;
}
 
