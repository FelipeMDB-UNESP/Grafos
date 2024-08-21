#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

typedef struct grafo {
    int V; //vertices
    int A; //arestas
    int **adj; //ponteiro para matriz de adjacência
}Grafo;


int** inicializa_matriz(int qtd_vertices){

    int** matriz_distancias = (int**) malloc(sizeof(int*));
    for (int i = 0; i < qtd_vertices; i++) {
        matriz_distancias[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }
    return matriz_distancias;
}



void djikstra(Grafo g, int** matriz_distancias, int qtd_vertices){

    int i, j = 0;
    int* vetor_visitados = (int*)malloc(sizeof(int) * qtd_vertices);
    for(i = 0; i < qtd_vertices; i++){
        vetor_visitados[i] = 0;
    }
    
    matriz_distancias[i][j] = 0;
    for(int i = 0; i < qtd_vertices; i++){
        matriz_distancias[1][i] = 
    }

}

void floyd(int** matriz_distancias){


}


int main(){
    Grafo g;
    int qtd_vertices;
    printf("Qual a quantidade de vertices?\n");
    scanf(" %d", &qtd_vertices);

    int** matriz_distancias = inicializa_matriz(qtd_vertices);
    djikstra(g, matriz_distancias, qtd_vertices);
    floyd(matriz_distancias);

}