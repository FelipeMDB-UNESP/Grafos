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

//TODO: Consertar quick_sort
void quick_sort_arestas(Aresta arestas[], int esq, int dir){
    int pivo = esq, i,ch,j;         
    for(i=esq+1;i<=dir;i++) {        
        j = i;                      
        if(arestas[j].peso < arestas[pivo].peso){     
            ch = arestas[j].peso;               
            while(j > pivo){           
                arestas[j].peso = arestas[j-1].peso;      
                j--;                    
            }
            arestas[j].peso = ch;               
            pivo++;                    
        }
    }
    if(pivo-1 >= esq) {              
        quick(arestas,esq,pivo-1);      
    }
    if(pivo+1 <= dir) {              
        quick(arestas,pivo+1,dir);      
    }
}



int kruskall(Aresta arestas[], int num_arestas) {

    bool floresta_geradora = false;
    quick_sort_arestas(arestas, arestas[0].peso, arestas[num_arestas - 1].peso);
    for (int i = 0; i < num_arestas && !floresta_geradora; i++) {
        int aresta_menor_peso = arestas[0];
    }
    


}



int main() {





    return 0;
}
