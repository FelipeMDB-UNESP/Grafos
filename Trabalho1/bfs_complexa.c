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

float bfs_arvore_binomial (int *arvore, int raiz, int qtd_nos) {

    // Vetor para armazenar as profundidades de cada nó
    float *profundidades = (float *) malloc(qtd_nos * sizeof(float));
    int pos_profundidades = 0;
    // Inicializa todas as profundidades com 0
    for (int i = 0; i < qtd_nos; i++) {
        profundidades[i] = 0;
    }

    //quantidade máxima de filhos por nó
    int max_filhos_por_no = arvore[0];

    //um bloco é representado pelo pai + filhos. Ou seja podemos iterar sobre os nós do vetor da árvore indo de bloco em bloco. 
    int bloco = max_filhos_por_no + 1;

    //fila de índices
    Fila *fila = inicializarFila(qtd_nos);

    profundidades[0] = 0;
    pos_profundidades++;
    enfileirar(fila, raiz);
    int grupos_dos_filhos[max_filhos_por_no];
    while(!estaVazia(fila)) {

        int no_atual = desenfileirar(fila);
        // printf("Visitando no: %d\n", (no_atual / bloco) + 1); // Visita o nó atual

        //i começa em 1 para podermos usar essa variável para encontrarmos o filho na posição correta, ignorando a info do pai.
        //no_atual - 1 -> para posicionarmos no começo das informações do nó, não em seu fim.
        //multiplicamos por bloco pois cada bloco representa um nó de forma abstrata.
        //soma-se i+1 no final pois queremos salvar os dados dos filhos. A primeira posição do bloco sempre armazena o índice de onde começa o bloco do pai do nó atual.
        bool tem_filho = false;
        for(int i = 0; i < max_filhos_por_no; i++) {

            grupos_dos_filhos[i] = arvore[no_atual + (i+1)];
            if (grupos_dos_filhos[i] != 0) {
                enfileirar(fila, grupos_dos_filhos[i]);

                profundidades[pos_profundidades] = profundidades[no_atual / bloco] + 1;
                pos_profundidades ++;
            }
        }
    } 
    
    int soma_profundidades = 0;
    for (int i = 0; i < qtd_nos; i++) {
        soma_profundidades += profundidades[i];
    }

    // Calcula a média das profundidades
    float media_profundidade = (float) soma_profundidades / qtd_nos;

    // Libera a memória da fila e do vetor de profundidades
    free(fila->data);
    free(fila);
    free(profundidades);
    free(grupos_dos_filhos);
    // Retorna a profundidade média
    return media_profundidade;
}


int main() {
    // Exemplo de uma árvore binomial representada como vetor
    int arvore[] = {3, 
                    1, 5, 9, 13, 
                    1, 17, 0, 0, 
                    1, 0, 0, 0, 
                    5, 0, 0, 0, 
                    6, 0, 0, 0};

    int qtd_nos = 5; // Número de nós

    float soma_profundidades = 0.0;
    int quantidade_de_raizes = 0;

    // Chama a função para cada nó como raiz
    for (int raiz = 1; raiz <= qtd_nos; raiz  = raiz + 4) {
        float profundidade_media = bfs_arvore_binomial(arvore, raiz, qtd_nos);
        printf("Profundidade media para a arvore com raiz = noh %d: %.2f\n", (raiz/4)+1, profundidade_media);
        soma_profundidades += profundidade_media; // Acumula a soma das profundidades
        quantidade_de_raizes++;
    }

    // Calcula a média das profundidades
    float media_total = soma_profundidades / quantidade_de_raizes;
    printf("Media total das profundidades: %.2f\n", media_total);

    return 0;
}