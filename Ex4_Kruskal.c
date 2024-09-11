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

typedef struct Arvore {

    Aresta *arestas;
    int num_arestas;
    int peso_total;
} ArvoreGeradoraMinima;


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
        for (int i = 0; i < matricial->n; i++) {
            for (int j = i; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    srand(time(NULL));
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

//quick sort ordena o vetor de arestas
//arestas[] -> vetor com todas as arestas do grafo
// peso_esq -> peso do "mais a esquerda"(início)
//peso_dir -> peso do "mais a direita"(final)
Aresta* quick_sort_arestas(Aresta *arestas, int esq, int dir) {

    int pivo = esq, i, j;

    for(i=esq+1;i<=dir;i++) {     

        j = i;                      
        if(arestas[j].peso < arestas[pivo].peso){    

            Aresta ch = arestas[j];               
            while(j > pivo){           
                arestas[j] = arestas[j-1];      
                j--;                    
            }
            arestas[j] = ch;               
            pivo++;                    
        }
    }
    if(pivo-1 >= esq) {    

        quick_sort_arestas(arestas,esq,pivo-1);      
    }
    if(pivo+1 <= dir) {

        quick_sort_arestas(arestas,pivo+1,dir);      
    }

    return arestas;
}

bool forma_ciclo(Aresta* arestas, int tam_vetor, Aresta nova_aresta) {

    for(int i = 0; i < tam_vetor; i++) {
        
        if(nova_aresta.v_origem == arestas[i].v_destino) {

            return true;
        }
    }
    return false;
}

ArvoreGeradoraMinima kruskal(Aresta *arestas, int num_arestas) {

    bool floresta_geradora = false;
    Aresta* arestas_arvore = criar_vetor_arestas(num_arestas);
    int tam_arvore = 0;
    int total_peso_arvore = 0;
    Aresta* arestas_ordenadas = quick_sort_arestas(arestas, 0, num_arestas - 1); //ordena o vetor de arestas por peso (crescente)
    int *vertices_visitados = (int*) malloc(sizeof(int) * num_arestas + 1); //assim garantimos que teremos um tamanho no mínimo igual a quantidade de vértices existentes

    int i;
    for (i = 0; i < num_arestas && !floresta_geradora; i++) {

        Aresta aresta_menor_peso = arestas_ordenadas[i];
        if(vertices_visitados[aresta_menor_peso.v_origem] && vertices_visitados[aresta_menor_peso.v_destino]) {

            continue;
        }else {

            vertices_visitados[aresta_menor_peso.v_destino] = 1; 
            vertices_visitados[aresta_menor_peso.v_origem] = 1; 
            arestas_arvore[tam_arvore] = aresta_menor_peso;
            tam_arvore++;
            total_peso_arvore += aresta_menor_peso.peso;
        }
    }

    ArvoreGeradoraMinima arvore = {arestas_arvore, tam_arvore, total_peso_arvore};
    free(vertices_visitados);
    return arvore;
}

#pragma endregion kruskal

#pragma region testes
void teste_ordenacao_arestas() {

    Aresta a = {1, 2, 10};
    Aresta b = {2,3, 5};
    Aresta c = {3, 4, 2};
    Aresta d = {4, 1, 7};

    Aresta arestas[] = {a, b, c, d};
    Aresta* arestasB = quick_sort_arestas(arestas, 0, 3);

    for(int i = 0; i < 4; i++){
        printf(" %d \n", arestasB[i].peso);
    }
}

#pragma endregion testes

int main() {

    int qtd_vertices, origem, direcionado, prob;
    printf("Qual a quantidade de vertices?\n");
    scanf(" %d", &qtd_vertices);

    prob = probabilidade();

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

    Aresta *arestas = criar_vetor_arestas(qtd_vertices * qtd_vertices);
    int qtd_arestas = 0;
    for(int i = 0; i < qtd_vertices; i++) {
        for(int j = 0; j < qtd_vertices; j++) {

            if(grafo->matriz[i][j]) {

                printf("Peso: %d, Origem: %d, Destino: %d\n", grafo->matriz[i][j], i, j);
                arestas[qtd_arestas].peso = grafo->matriz[i][j];
                arestas[qtd_arestas].v_origem = i;
                arestas[qtd_arestas].v_destino = j;
                qtd_arestas++;
            }
        }
    }

    // kruskal
    ArvoreGeradoraMinima arvore = kruskal(arestas, qtd_arestas);

    // Exibe o resultado de kruskal

    for(int i = 0; i < arvore.num_arestas; i++) {
        printf(" Origem: %d, Destino: %d, Peso: %d\n", arvore.arestas[i].v_origem, arvore.arestas[i].v_destino, arvore.arestas[i].peso);
    }

    // Libera a memória utilizada
    liberar_matriz(grafo);
    liberar_vetor_arestas(arestas);
    return 0;
}
