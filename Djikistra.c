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



int* djikstra(int** matriz_distancias, int qtd_vertices, int origem) {
    int* vetor_distancias = (int*)malloc(sizeof(int) * qtd_vertices);
    int* vetor_visitados = (int*)malloc(sizeof(int) * qtd_vertices);
    int i, j;

    // Inicializa todos os vértices como não visitados e as distâncias como maior número possível
    for (i = 0; i < qtd_vertices; i++) {
        vetor_distancias[i] = INT_MAX;
        vetor_visitados[i] = 0;
    }

    // A distância da origem para si mesma é 0
    vetor_distancias[origem] = 0;

    //percorre todos os vértices
    for (i = 0; i < qtd_vertices - 1; i++) {
        int min_dist = INT_MAX;
        int u = -1;

        // Encontra o vértice não visitado com a menor distância
        for (j = 0; j < qtd_vertices; j++) {
            if (!vetor_visitados[j] && vetor_distancias[j] < min_dist) {
                min_dist = vetor_distancias[j];
                u = j;
            }
        }

        // Marca o vértice atual como visitado
        vetor_visitados[u] = 1;

        // Atualiza as distâncias dos vértices adjacentes
        for (j = 0; j < qtd_vertices; j++) {
            //se o vértice j não foi visitado, existe um caminho de u para j e se a distância uj é menor que a distância atual guardada
            if (!vetor_visitados[j] && matriz_distancias[u][j] && vetor_distancias[u] != INT_MAX &&
                vetor_distancias[u] + matriz_distancias[u][j] < vetor_distancias[j]) {
                vetor_distancias[j] = vetor_distancias[u] + matriz_distancias[u][j];
            }
        }
    }

    // Libera o vetor de visitados
    free(vetor_visitados);

    // Retorna o vetor de distâncias
    return vetor_distancias;
}


int main(){
    Grafo g;
    int qtd_vertices, origem;
    printf("Qual a quantidade de vertices?\n");
    scanf(" %d", &qtd_vertices);
    
    printf("Qual a origem? O ponto de onde se pretende sair?\n");
    scanf(" %d", &origem);

    int** matriz_distancias = inicializa_matriz(qtd_vertices);
    int* distancias = djikstra(g, matriz_distancias, qtd_vertices, origem);
    
    if (distancias != NULL) {
        // Exibe as distâncias da origem para todos os vértices
        for (int i = 0; i < qtd_vertices; i++) {
            printf("Distância do vértice %d até %d: %d\n", origem, i, distancias[i]);
        }
        free(distancias);
    }

}