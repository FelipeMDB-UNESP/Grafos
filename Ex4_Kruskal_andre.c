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

#pragma region matrizes

typedef struct matriz {

    int n;          //linhas
    int **matriz; //ponteiro para matriz, no caso desse exercício é uma matriz de adjacências
} Matriz;

typedef struct aresta {
    int v_origem;
    int v_destino;
    int peso;
} Aresta;
/*
typedef struct Arvore {

    Aresta *arestas;
    int num_arestas;
    int peso_total;
} ArvoreGeradoraMinima;
*/


Matriz* inicializar_matriz(int qtd_vertices) {

    Matriz* matricial = (Matriz*) malloc(sizeof(Matriz));
    int** matriz_adjascencia = (int**) malloc(qtd_vertices * sizeof(int*));
    for (int i = 0; i < qtd_vertices; i++) {
        matriz_adjascencia[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }
    matricial->n = qtd_vertices;
    matricial->matriz = matriz_adjascencia;
    return matricial;
}

void liberar_matriz(Matriz* matricial) {

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
        srand(time(NULL)); 
        for (int i = 0; i < matricial->n; i++) {
            for (int j = i; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    matricial->matriz[i][j] = (rand() % 100 < probabilidade) ? 1  * (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
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

#pragma region vetores
Aresta* criar_vetor_arestas(int n) {

    Aresta* vetor = (Aresta*)malloc(sizeof(Aresta) * n);
    return vetor;
}

void liberar_vetor_arestas(Aresta* vetor) {

    free(vetor);
}
#pragma endregion vetores

#pragma region kruskal

int particiona(Aresta *arestas, int esq, int dir) {
    Aresta pivo = arestas[dir];
    int i = esq - 1;
    for (int j = esq; j < dir; j++) {
        if (arestas[j].peso < pivo.peso) {
            i++;
            Aresta temp = arestas[i];
            arestas[i] = arestas[j];
            arestas[j] = temp;
        }
    }
    Aresta temp = arestas[i + 1];
    arestas[i + 1] = arestas[dir];
    arestas[dir] = temp;
    return i + 1;
}

void quick_sort_arestas(Aresta *arestas, int esq, int dir) {
    if (esq < dir) {
        int pivo = particiona(arestas, esq, dir);
        quick_sort_arestas(arestas, esq, pivo - 1);
        quick_sort_arestas(arestas, pivo + 1, dir);
    }
}

//TODO: UNION-FIND
/*
ArvoreGeradoraMinima kruskal(Aresta *arestas, int num_arestas) {
    ArvoreGeradoraMinima arvore = {criar_vetor_arestas(num_arestas), 0, 0};
    int tam_arvore = 0;

    quick_sort_arestas(arestas, 0, num_arestas - 1);

    for (int i = 0; i < num_arestas; i++) {
        Aresta aresta_menor_peso = arestas[i];
        if (vertices_visitados[aresta_menor_peso.v_origem] && vertices_visitados[aresta_menor_peso.v_destino]) {
            continue;
        }
        vertices_visitados[aresta_menor_peso.v_destino] = 1; 
        vertices_visitados[aresta_menor_peso.v_origem] = 1; 
        arvore.arestas[tam_arvore++] = aresta_menor_peso;
        arvore.peso_total += aresta_menor_peso.peso;
    }

    arvore.num_arestas = tam_arvore;
    free(vertices_visitados);
    return arvore;
}
*/
int pai_verdadeiro(int* pai, int pos) {
    int aux = pos;
    while (pai[aux] != aux) {
        aux = pai[aux];
    }
    return aux;
}

int * kruskal(Aresta *aresta, int qtd_arestas, int qtd_vertices) {

    int * marcador_aresta = (int*) calloc(qtd_arestas,sizeof(int));
    int * visitado = (int*) calloc(qtd_vertices,sizeof(int));
    int * pai = (int*) malloc(sizeof(int)*qtd_vertices);
    bool ocorrencia = true;
    

    quick_sort_arestas(aresta, 0, qtd_arestas-1);

    int aux, temp;

    for(int i = 0; i<qtd_arestas;i++) {

        if (visitado[aresta[i].v_origem] && visitado[aresta[i].v_destino]) {
            temp = pai_verdadeiro(pai,aresta[i].v_origem);
            aux = pai_verdadeiro(pai, aresta[i].v_destino);
            if (temp != aux) {
                pai[aux] = temp;
                marcador_aresta[i]=1;
            }
            continue;
        }

        if (!visitado[aresta[i].v_origem] && !visitado[aresta[i].v_destino]) {
            pai[aresta[i].v_origem] = aresta[i].v_origem;
            pai[aresta[i].v_destino] = aresta[i].v_origem;
        } else {
            if (visitado[aresta[i].v_origem]) {
                pai[aresta[i].v_destino] = pai[aresta[i].v_origem];
            } else {
                pai[aresta[i].v_origem] = pai[aresta[i].v_destino];
            }
        }

        visitado[aresta[i].v_origem] = 1;
        visitado[aresta[i].v_destino] = 1;
        marcador_aresta[i]=1;
    }
    free(visitado);
    free(pai);
    return marcador_aresta;
}

void leitura_kruskal(Aresta* aresta, int * marcador_aresta, int qtd_arestas) {
    for(int i = 0; i < qtd_arestas; i++) {
        if (marcador_aresta[i])
            printf(" Origem: %d, Destino: %d, Peso: %d\n", aresta[i].v_origem, aresta[i].v_destino, aresta[i].peso);
    }
}

#pragma endregion kruskal

#pragma region testes
void teste_ordenacao_arestas() {

    Aresta a = {1, 2, 10};
    Aresta b = {2,3, 5};
    Aresta c = {3, 4, 2};
    Aresta d = {4, 1, 7};

    Aresta arestas[] = {a, b, c, d};
    quick_sort_arestas(arestas, 0, 3);

    for(int i = 0; i < 4; i++){
        printf(" %d \n", arestas[i].peso);
    }
}

#pragma endregion testes

int main() {

    int qtd_vertices, origem, direcionado, prob;
    printf("Qual a quantidade de vertices?\n");
    scanf(" %d", &qtd_vertices);



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
        prob = probabilidade();
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

    
    Aresta *arestas = criar_vetor_arestas(qtd_vertices * (qtd_vertices-1));
    int qtd_arestas = 0;
    for(int i = 0; i < qtd_vertices; i++) {
        for(int j = 0; j < qtd_vertices; j++) {

            if(grafo->matriz[i][j]) {

                //printf("Peso: %d, Origem: %d, Destino: %d\n", grafo->matriz[i][j], i, j);
                arestas[qtd_arestas].peso = grafo->matriz[i][j];
                arestas[qtd_arestas].v_origem = i;
                arestas[qtd_arestas].v_destino = j;
                qtd_arestas++;
            }
        }
    }
    
    /*
    // kruskal
    ArvoreGeradoraMinima arvore = kruskal(arestas, qtd_arestas, qtd_vertices);

    // Exibe o resultado de kruskal

    for(int i = 0; i < arvore.num_arestas; i++) {
        printf(" Origem: %d, Destino: %d, Peso: %d\n", arvore.arestas[i].v_origem, arvore.arestas[i].v_destino, arvore.arestas[i].peso);
    }
    */

    int * marcador = kruskal(arestas, qtd_arestas, qtd_vertices);

    leitura_kruskal(arestas, marcador, qtd_arestas);

    // Libera a memória utilizada
    liberar_matriz(grafo);
    liberar_vetor_arestas(arestas);
    free(marcador);
    return 0;
}
