/*
Por:
André Luis Dias Nogueira
Felipe Melchior de Britto
Rafael Daiki Kaneko
Ryan Hideki Tadeo Guimarães
Vitor Marchini Rolisola
*/
//algoritmo de busca em profundidade para ordenação topológica


#include <stdio.h>
#include <stdlib.h>

typedef struct grafo {
    int V; //vertices
    int A; //arestas
    int **adj; //ponteiro para matriz de adjacência
} Grafo;

typedef struct no {
    int dado;              // dado do nó
    struct no **filhos;    // Ponteiro para os filhos
    int numFilhos;         // Número de filhos
} No;

typedef struct arvore {
    No *raiz;              // Ponteiro para a raiz da árvore
    int numNos;            // Número total de nós na árvore
} Arvore;


void busca_em_profundidade(Arvore arv){

}


int main(){

}