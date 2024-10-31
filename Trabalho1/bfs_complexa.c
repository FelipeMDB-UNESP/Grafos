#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"

typedef struct {
    int* data;
    int entrada, saida, tamanho;
} Fila;

Fila* inicializarFila(int max_size) {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    f->data = (int*)malloc(max_size * sizeof(int));
    f->entrada = 0;
    f->saida = 0;
    f->tamanho = max_size;
    return f;
}

void enfileirar(Fila* f, int value) {
    if (f->saida < f->tamanho) {
        f->data[f->saida++] = value;
    }
}

int desenfileirar(Fila* f) {
    if (f->entrada < f->saida) {
        return f->data[f->entrada++];
    }
    return -1; // Fila vazia
}

int estaVazia(Fila* f) {
    return f->entrada == f->saida;
}

int bfs_arvore_binomial (int *arvore, int raiz, int qtd_nos) {

    // Vetor para armazenar as profundidades de cada nó
    float *profundidades = (float *) malloc(qtd_nos * sizeof(float));
    //precisávamos iterar em profundidades de forma sequencial, essa é a maneira mais simples;
    int pos_profundidades = 0;
    //Marca os nós visitados
    int *visitados = (int *) calloc(qtd_nos, sizeof(int));
    // Inicializa todas as profundidades com 0
    for (int i = 0; i < qtd_nos; i++) {
        profundidades[i] = 0;
    }

    //um bloco é representado pelo pai + filhos. Ou seja podemos iterar sobre os nós do vetor da árvore indo de bloco em bloco. 
    int bloco = arvore[0] + 1;
    //fila de índices
    Fila *fila = inicializarFila(qtd_nos);

    //profundidade inicial é zero
    profundidades[raiz / bloco] = 0;
    enfileirar(fila, raiz);
    visitados[(raiz / bloco)] = 1;

    int no_anterior;
    while(!estaVazia(fila)) {

        int no_atual = desenfileirar(fila);
        visitados[no_atual / 4] = 1;
        // printf("Visitando no: %d\n", (no_atual / bloco) + 1); // Visita o nó atual

        //i começa em 1 para podermos usar essa variável para encontrarmos o filho na posição correta, ignorando a info do pai.
        //no_atual - 1 -> para posicionarmos no começo das informações do nó, não em seu fim.
        //multiplicamos por bloco pois cada bloco representa um nó de forma abstrata.
        //soma-se i+1 no final pois queremos salvar os dados dos filhos. A primeira posição do bloco sempre armazena o índice de onde começa o bloco do pai do nó atual.
        int pai_ou_filho;
        for(int i = 0; i < bloco; i++) {

            pai_ou_filho = arvore[no_atual + i];
            if (pai_ou_filho != 0 && !visitados[(pai_ou_filho / bloco)]) {
                enfileirar(fila, pai_ou_filho);

                profundidades[pai_ou_filho / bloco] = profundidades[no_atual / bloco] + 1;
            }
        }
    } 

    // Calcula a média das profundidades
    int maior_profundidade = 0;
    for(int i = 0; i < qtd_nos; i++) {
        if(profundidades[i] > maior_profundidade)
            maior_profundidade = profundidades[i];
    }

    // Libera a memória da fila e do vetor de profundidades
    free(fila->data);
    free(fila);
    free(profundidades);
    // Retorna a profundidade média
    return maior_profundidade;
}

int main() {
    // Exemplo de uma árvore binomial representada como vetor
    int arvore[] = {3, 
                    1, 5, 9, 13, 
                    1, 17, 0, 0, 
                    1, 0, 0, 0, 
                    5, 0, 0, 0, 
                    6, 21, 0, 0,
                    17, 0, 0, 0};

    int qtd_nos = 6; // Número de nós
    int bloco = 4;

    float soma_profundidades = 0.0;

    // Chama a função para cada nó como raiz
    //quantidade de filhos + 1 é como se fôssemos de índice a índice.
    // no meio do for é bloco - 1
    for (int raiz = 1; raiz <= ((qtd_nos-1) * bloco) + 1; raiz  = raiz + bloco) {
        int profundidade = bfs_arvore_binomial(arvore, raiz, qtd_nos);
        printf("Profundidade da arvore com raiz = noh %d: %d\n", (raiz/bloco)+1, profundidade);
        soma_profundidades += (float)profundidade; // Acumula a soma das profundidades
    }

    // Calcula a média das profundidades
    float media_total = soma_profundidades / qtd_nos;
    printf("Media total das profundidades: %.2f\n", media_total);

    return 0;
}