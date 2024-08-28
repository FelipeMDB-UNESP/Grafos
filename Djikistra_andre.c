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

typedef struct matricial {
    int n;     //linhas
    int **matriz; //ponteiro para matriz
}Matriz;

Matriz* inicializar_matriz(int n){

    Matriz* matricial = (Matriz*) malloc(sizeof(Matriz));
    int** matriz = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matriz[i] = (int*)malloc(n * sizeof(int));
    }
    matricial->n = n;
    matricial->matriz = matriz;
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

int* criar_vetor(int n) {
    int* vetor = (int*)malloc(sizeof(int) * n);
    return vetor;
}

void liberar_vetor(int* vetor) {
    free(vetor);
}

int* djikstra(Matriz* grafo, int origem) {

    int* vetor_distancias = criar_vetor(grafo->n);
    int* vetor_visitados = criar_vetor(grafo->n);
    int i, j;

    // Verifica se a alocação de memória foi bem-sucedida
    if (vetor_distancias == NULL || vetor_visitados == NULL) {

        printf("Erro ao alocar memoria.\n");
        return NULL;
    }

    // Inicializa todos os vértices como não visitados e as distâncias como infinito
    for (i = 0; i < grafo->n; i++) {
        vetor_distancias[i] = INT_MAX;
        vetor_visitados[i] = 0;
    }

    // A distância da origem para si mesma é 0
    vetor_distancias[origem] = 0;

    // Loop principal: percorre todos os vértices
    for (i = 0; i < grafo->n - 1; i++) {

        int min_dist = INT_MAX;
        int u = -1;

        // Encontra o vértice não visitado com a menor distância
        for (j = 0; j < grafo->n; j++) {

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
        for (j = 0; j < grafo->n; j++) {

            //se o vértice não foi visitado e a matriz de distanciâs de uj
            if (!vetor_visitados[j] && 
                grafo->matriz[u][j] && 
                vetor_distancias[u] != INT_MAX &&
                vetor_distancias[u] + grafo->matriz[u][j] < vetor_distancias[j]
                ) {
                    
                vetor_distancias[j] = vetor_distancias[u] + grafo->matriz[u][j];
            }
        }
    }

    // Libera o vetor de visitados
    liberar_vetor(vetor_visitados);

    // Retorna o vetor de distâncias
    return vetor_distancias;
}



int main() {

    int qtd_vertices, origem, direcionado, prob;
    printf("Qual a quantidade de vertices?\n");
    scanf("%d", &qtd_vertices);

    printf("Qual a origem? O ponto de onde se pretende sair?\n");
    scanf("%d", &origem);

    // Verifica se a origem está dentro do intervalo válido
    if (origem < 0 || origem >= qtd_vertices) {

        printf("Origem invalida.\n");
        return 1;
    }

    //verifica se eh orientado ou nao
    do {

        printf("\nSelecione:\n0 - Grafo NAO direcionado\n1 - Grafo direcionado\n");
        scanf("%d", &direcionado);

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
        
        gerar_grafo(grafo, direcionado);

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
                    scanf("%d", &grafo->matriz[i][j]);
                }
                else if (i < j && !direcionado){

                    printf("Distancia entre o vertice %d e %d: ", i, j);
                    scanf("%d", &grafo->matriz[i][j]);
                    grafo->matriz[j][i] = grafo->matriz[i][j];
                }

                if (grafo->matriz[i][j] == -1) {

                    grafo->matriz[i][j] = 0; // Considerando 0 para ausência de aresta
                }
            }
        }
    }
    


    int* distancias = djikstra(grafo, origem);

    if (distancias != NULL) {

        printf("\n\n\n-------DISTANCIAS-------\n\n");
        // Exibe as distâncias da origem para todos os vértices
        for (int i = 0; i < grafo->n; i++) {

            if(origem != i){
                printf("Distancia do vertice %d ate %d: %d\n", origem, i, distancias[i]);
            }
        }
        liberar_vetor(distancias);
    }

    // Libera a memória da matriz de distâncias
    liberar_matriz(grafo);

    return 0;
}
