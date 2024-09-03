#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

typedef struct grafo {
    int V; //vertices
    int A; //arestas
    int **adj; //ponteiro para matriz de adjacência
} Grafo;

int** inicializa_matriz(int qtd_vertices) {

    int** matriz_distancias = (int**) malloc(qtd_vertices * sizeof(int*));
    for (int i = 0; i < qtd_vertices; i++) {

        matriz_distancias[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }
    return matriz_distancias;
}




int* djikstra(int** matriz_distancias, int qtd_vertices, int origem) {

    int* vetor_distancias = (int*)malloc(sizeof(int) * qtd_vertices);
    int* vetor_visitados = (int*)malloc(sizeof(int) * qtd_vertices);
    int i, j;

    // Verifica se a alocação de memória foi bem-sucedida
    if (vetor_distancias == NULL || vetor_visitados == NULL) {

        printf("Erro ao alocar memoria.\n");
        return NULL;
    }

    // Inicializa todos os vértices como não visitados e as distâncias como infinito
    for (i = 0; i < qtd_vertices; i++) {

        vetor_distancias[i] = INT_MAX;
        vetor_visitados[i] = 0;
    }

    // A distância da origem para si mesma é 0
    vetor_distancias[origem] = 0;

    // Loop principal: percorre todos os vértices
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

        // Se u não foi atualizado, todos os vértices acessíveis já foram visitados
        if (u == -1) {
            break;
        }

        // Marca o vértice atual como visitado
        vetor_visitados[u] = 1;

        // Atualiza as distâncias dos vértices adjacentes
        for (j = 0; j < qtd_vertices; j++) {

            //se o vértice não foi visitado e a matriz de distanciâs de uj
            if (!vetor_visitados[j] && 
                matriz_distancias[u][j] && 
                vetor_distancias[u] != INT_MAX &&
                vetor_distancias[u] + matriz_distancias[u][j] < vetor_distancias[j]
                ) {
                    
                vetor_distancias[j] = vetor_distancias[u] + matriz_distancias[u][j];
            }
        }
    }

    // Libera o vetor de visitados
    free(vetor_visitados);

    // Retorna o vetor de distâncias
    return vetor_distancias;
}



int main() {

    Grafo g;
    int qtd_vertices, origem, direcionado;
    printf("Qual a quantidade de vertices?\n");
    scanf(" %d", &qtd_vertices);

    printf("Qual a origem? O ponto de onde se pretende sair?\n");
    scanf(" %d", &origem);

    // Verifica se a origem está dentro do intervalo válido
    if (origem < 0 || origem >= qtd_vertices) {

        printf("Origem invalida.\n");
        return 1;
    }

    int** matriz_distancias = inicializa_matriz(qtd_vertices);
    
    do {

        printf("\nSelecione:\n0 - Grafo NAO direcionado\n1 - Grafo direcionado\n");
        scanf(" %d", &direcionado);

        if (direcionado != 0 && direcionado != 1) {
            printf("\nOPCAO INVALIDA!!!\n");
        }
    } while (direcionado != 0 && direcionado != 1);


    // Preenche a matriz de distâncias
    printf("Informe as distancias entre os vertices (use -1 para indicar ausencia de aresta):\n");
    for (int i = 0; i < qtd_vertices; i++) {

        for (int j = 0; j < qtd_vertices; j++) {

            if (i == j){

                matriz_distancias[i][j] = 0;
            }
            else if(direcionado) {

                printf("Distancia entre o vertice %d e %d: ", i, j);
                scanf(" %d", &matriz_distancias[i][j]);
            }
            else if (i < j && !direcionado){

                printf("Distancia entre o vertice %d e %d: ", i, j);
                scanf(" %d", &matriz_distancias[i][j]);
                matriz_distancias[j][i] = matriz_distancias[i][j];
            }

            if (matriz_distancias[i][j] == -1) {

                matriz_distancias[i][j] = 0; // Considerando 0 para ausência de aresta
            }
        }
    }

    int* distancias = djikstra(matriz_distancias, qtd_vertices, origem);

    if (distancias != NULL) {

        printf("\n\n\n-------DISTANCIAS-------\n\n");
        // Exibe as distâncias da origem para todos os vértices
        for (int i = 0; i < qtd_vertices; i++) {

            if(origem != i){
                printf("Distancia do vertice %d ate %d: %d\n", origem, i, distancias[i]);
            }
        }
        free(distancias);
    }

    // Libera a memória da matriz de distâncias
    for (int i = 0; i < qtd_vertices; i++) {

        free(matriz_distancias[i]);
    }
    free(matriz_distancias);

    return 0;
}