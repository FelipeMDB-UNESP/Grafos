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
#include <math.h>
#include <time.h>
#include <limits.h>

#pragma region vetores
int* criar_vetor(int n) {

    int* vetor = (int*)malloc(sizeof(int) * n);
    return vetor;
}

void liberar_vetor(int* vetor) {

    free(vetor);
}
#pragma endregion vetores

#pragma region filas
typedef struct dado {

    int dado;
    struct dado* proximo;
} DADO;

typedef struct {

    DADO* entrada;
    DADO* saida;
} Fila;

typedef Fila* p_fila;

p_fila criar_fila() {

    p_fila f = malloc(sizeof(Fila));
    f->entrada = NULL;
    f->saida = NULL;
    return f;
}

int fila_vazia(p_fila f){

    return (f->saida == NULL);
}

void esvaziar_fila(p_fila f){

    DADO* aux;
    while(!fila_vazia(f)) {
        aux = f->saida;
        f->saida = f->saida->proximo;
        free(aux);
    }
    f->entrada = NULL;
}

void liberar_fila(p_fila f){

    DADO* aux;
    while(!fila_vazia(f)) {
        aux = f->saida;
        f->saida = f->saida->proximo;
        free(aux);
    }
    free(f);
}

void enfileirar(p_fila f, int k) {

    DADO* aux = malloc(sizeof(DADO));
    aux->dado = k;
    aux->proximo = NULL;

    if (!fila_vazia(f)) {
        f->entrada->proximo = aux;
        f->entrada = aux;
    } else {
        f->entrada = aux;
        f->saida = aux;
    }
}

int desenfileirar(p_fila f) {

    DADO* aux;
    int i;
    if (!fila_vazia(f)) {
        aux = f->saida;
        if (f->entrada != f->saida) {
            f->saida = f->saida->proximo;
        } else {
            f->entrada = NULL;
            f->saida = NULL;
        }
        i = aux->dado;
        free(aux);
        return i;
    }
    return INT_MIN;
}
#pragma endregion filas

#pragma region arvore

typedef long int big_value;

big_value estimar_nos(int qtd_filhos, float prob, int depth) {
    
    big_value qtd_vertices = 0;
    float constante = qtd_filhos * prob;
    
    if (constante < 1)
        return (big_value) ceil(1 / (1 - constante));

    for (int x = 0; x<=depth; x++) {
        qtd_vertices += pow(qtd_filhos, x*(x+1)/2);
    }
    return (big_value) qtd_vertices;
}

/*
Inicializa a arvore vetorizada:
qtd_filhos -> quantidade de filhos que um vertice pode ter
qtd_vertices -> quantidade maxima de vertices da arvore a ser criada*/
int* inicializar_arvore(int qtd_filhos, float prob, int depth) {
    int* arvore = (int*) calloc(((qtd_filhos+1)* estimar_nos(qtd_filhos, prob, depth) + 1), sizeof(big_value));
    arvore[0] = qtd_filhos;
    /*for (big_value i = 1; i<((qtd_filhos+1)* estimar_nos(qtd_filhos, prob, depth) + 1); i++)
        arvore[i] = 0;*/
    return arvore;
}

/*
Realiza a liberacao da memoria separada para a arvore*/
void liberar_arvore(int* arvore) {
    free(arvore);
}

/*
Cria uma camada de abstracao, para que o usuario nao se preocupe com o manejo dos indices do vetor
valor_real -> valor original da posicao do vetor
qtd_filhos -> quantidade de filhos que um vertice pode ter
*/
int abstracao(int valor_real, int qtd_filhos) {
    int valor_abstrato = 1 + (valor_real-1)/(qtd_filhos+1);
    return valor_abstrato;
}

/*
Desfaz a camada de abstracao, convertendo para o valor real na posicao do vetor
valor_abstrato -> valor abstraido da posicao dos vertices no vetor
qtd_filhos -> quantidade de filhos que um vertice pode ter*/
int concretizacao(int valor_abstrato, int qtd_filhos) {
    int valor_real = 1 + (valor_abstrato-1)*(qtd_filhos+1);
    return valor_real;
}

/*
Retorna o pai do vertice inserido (ja abstraido)
arvore -> vetor de inteiros que estamos usando para trabalhar com uma arvore
vertice -> valor abstraido da posicao real dos dados do vertice no vetor*/
int pai(int* arvore, int vertice) {
    int valor_real = concretizacao(vertice, arvore[0]);
    return abstracao(arvore[valor_real],arvore[0]);
}

/*
Retorna o filho do vertice inserido (ja abstraido)
arvore -> vetor de inteiros que estamos usando para trabalhar com uma arvore
vertice -> valor abstraido da posicao real dos dados do vertice no vetor
filho -> numero do filho em questao
*/
int filho(int* arvore, int vertice, int filho) {
    int valor_real = concretizacao(vertice, arvore[0]);
    return abstracao(arvore[valor_real + filho],arvore[0]);
}


// BIG RYAN NOTES 2.0
int* gerar_arvore(int* arvore, int qtd_filhos_max, float prob_filho, int profundidade, big_value max_noh_possivel){
    srand(time(NULL));
    big_value i = 1;
    int j = 1;
    int qtd_noh = 1;
    bool criou = false;
    int indice;
    arvore[i] = 1; 
    printf("prob do filho: %.2f\n", prob_filho);
    printf("|Pai %d", arvore[i]);
    for(i = 2; i < max_noh_possivel; i++){
        // if (i == 1 + ((qtd_filhos_max + 1) * qtd_noh)){
        if (i == (i/(qtd_filhos_max+1))*(qtd_filhos_max+1) + 1){
            if (arvore[i] == 0){
                printf("\nFim da linhagem!");
                break;
            }
            j = j + 5;
            printf("\n|Pai %ld", i);
            continue;
        }
        int range_teste = rand();
        criou = rand() % 100 < ((int) (prob_filho*100)) ? true : false;
        if (criou){
            indice = 1 + ((qtd_filhos_max + 1) * qtd_noh);
            arvore[i] = indice;
            arvore[indice] = j;
            qtd_noh++;
            
        }
        printf("|%d", arvore[i]);
        j++;
    }
    printf("\n%ld\n",i);
    printf("%ld\n",max_noh_possivel);
    //Isso é um easter egg! +10 pontos //(i/(qtd_filhos+1))*(qtd_filhos+1) + 1
}


#pragma endregion arvore

#pragma region matrizes
typedef struct matricial {

    int n;          //linhas
    int **matriz; //ponteiro para matriz
}Matriz;

Matriz* inicializar_matriz(int qtd_vertices){

    Matriz* matricial = (Matriz*) malloc(sizeof(Matriz));
    int** matriz_adjascencia = (int**) malloc(qtd_vertices * sizeof(int*));
    for (int i = 0; i < qtd_vertices; i++) {
        matriz_adjascencia[i] = (int*)malloc(qtd_vertices * sizeof(int));
    }
    matricial->n = qtd_vertices;
    matricial->matriz = matriz_adjascencia;
    return matricial;
}

void liberar_matriz(Matriz* matricial){

    for (int i = 0; i < matricial->n; i++) {
        free(matricial->matriz[i]);
    }
    free(matricial->matriz);
    free(matricial);
}
#pragma endregion matrizes

#pragma region grafo_aleatorio
float probabilidade() {

    float prob;
    do {
        
        printf("Qual a probabilidade de cada aresta? (valores entre 0 e 1)\n");
        
        // Lê a probabilidade como um número de ponto flutuante
        scanf(" %f", &prob);
        
        // Validação para garantir que o valor está entre 0 e 1
        if (prob < 0 || prob > 1) {

            printf("Erro: Valor inválido! Insira um número entre 0 e 1.\n");
        }
    }while(prob < 0 || prob > 1);
    return prob;
}

//função que gera um grafo com arestas aleatorias
void gerar_grafo(Matriz* matricial, bool orientado, int probabilidade) {
    
    
    if (!orientado) {   //garante espelhamento 
        for (int i = 0; i < matricial->n; i++) {
            for (int j = i; j < matricial->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    srand(time(NULL));
                    matricial->matriz[i][j] = (rand() % 100 < probabilidade) ? (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
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

int* busca_em_largura(Matriz* grafo, int origem) {

    int i, j;
    int* pai = criar_vetor(grafo->n);
    int* visitado = criar_vetor(grafo->n);
    p_fila f = criar_fila();

    for (i=0; i < grafo->n; i++) {
        pai[i]= -1;
        visitado[i] = 0;
    }
    
    enfileirar(f,origem);
    pai[origem] = origem;
    visitado[origem] = 1;

    while (!fila_vazia(f)) {
        for (i = desenfileirar(f), j = 0; j < grafo->n; j++) {
            if (grafo->matriz[i][j] && !visitado[j]) {
                visitado[j] = 1;
                pai[j] = i;
                enfileirar(f, j);
            }
        }
    }

    liberar_fila(f);
    liberar_vetor(visitado);
    return pai;
}

int main() {

    // int qtd_vertices, origem, direcionado, prob;
    // printf("Qual a quantidade de vertices?\n");
    // scanf(" %d", &qtd_vertices);

    // printf("Qual o vertice de origem?\n");
    // scanf(" %d", &origem);

    // // Verifica se a origem está dentro do intervalo válido
    // if (origem < 0 || origem >= qtd_vertices) {

    //     printf("Origem invalida.\n");
    //     return 1;
    // }

    // //verifica se eh direcionado ou nao
    // do {

    //     printf("\nSelecione:\n0 - Grafo NAO direcionado\n1 - Grafo direcionado\n");
    //     scanf(" %d", &direcionado);

    //     if (direcionado != 0 && direcionado != 1) {
    //         printf("\nOPCAO INVALIDA!!!\n");
    //     }
    // } while (direcionado != 0 && direcionado != 1);


    // Matriz* grafo = inicializar_matriz(qtd_vertices);


    // int aux;
    // do {

    //     printf("\nDeseja um grafo aleatorio? [1=SIM/0=NAO]\n");
    //     scanf(" %d", &aux);

    //     if (aux != 0 && aux != 1) {
    //         printf("\nOPCAO INVALIDA!!!\n");
    //     }
    // } while (aux != 0 && aux != 1);

    // if (aux) {
    //     prob = probabilidade();
    //     gerar_grafo(grafo, direcionado, (int)(probabilidade() * 100));
    // } else {

    //     // Preenche a matriz de distâncias
    //     printf("Informe as distancias entre os vertices (use -1 para indicar ausencia de aresta):\n");
    //     for (int i = 0; i < grafo->n; i++) {

    //         for (int j = 0; j < grafo->n; j++) {

    //             if (i == j){

    //                 grafo->matriz[i][j] = 0;
    //             }
    //             else if(direcionado) {

    //                 printf("Distancia entre o vertice %d e %d: ", i, j);
    //                 scanf(" %d", &grafo->matriz[i][j]);
    //             }
    //             else if (i < j && !direcionado){

    //                 printf("Distancia entre o vertice %d e %d: ", i, j);
    //                 scanf(" %d", &grafo->matriz[i][j]);
    //                 grafo->matriz[j][i] = grafo->matriz[i][j];
    //             }

    //             if (grafo->matriz[i][j] == -1) {

    //                 grafo->matriz[i][j] = 0; // Considerando 0 para ausência de aresta
    //             }
    //         }
    //     }
    // }

    // // Realiza a busca em largura
    // int* resultado = busca_em_largura(grafo, origem);

    // // Exibe o vetor de pais retornado pela busca em largura
    // printf("\nVetor de pais na busca em largura a partir do vertice %d:\n", origem);
    // for (int i = 0; i < grafo->n; i++) {
    //     printf("Vertice %d: Pai = %d\n", i, resultado[i]);
    // }

    // Libera a memória utilizada
    // liberar_vetor(resultado);
    // liberar_matriz(grafo);

    int qtd_filhos = 6;
    float prob = 0.8;
    int depth = 4;
    int* arvore_testes = inicializar_arvore(qtd_filhos, prob, depth);
    printf("%ld\n", estimar_nos(qtd_filhos, prob, depth));
    //gerar_arvore(arvore_testes, qtd_filhos, prob, depth, estimar_nos(qtd_filhos, prob, depth)); // USANDO 100 TEMPORÁRIAMENTE!

    return 0;
}
