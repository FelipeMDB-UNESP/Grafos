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

#define ITERACOES 10

#pragma region vetores
/*
Função responsável por criar dinamincamente um vetor.

Parâmetros:
int n -> tamanho do vetor

Retorno:
int* vetor -> vetor dinamicamente alocado
*/
int* criar_vetor(int n) {

    int* vetor = (int*)malloc(sizeof(int) * n);
    return vetor;
}

/*
Função responsável por liberar o espaço alocado a um vetor.

Parâmetros:
int* vetor -> ponteiro do vetor a ser liberado

Retorno: void
*/
void liberar_vetor(int* vetor) {

    free(vetor);
}
#pragma endregion vetores

#pragma region filas
/*
Estrutura de um dado idealmente a ser utilizado em filas.

int dado -> inteiro que representa o valor dessa estrutura.
struct dado* proximo -> ponteiro para o próximo dado.
*/
typedef struct dado {

    int dado;
    struct dado* proximo;
} DADO;

/*
Estrutura de fila (FIFO).

DADO* entrada -> Entrada na fila, ou seja, última posição da fila.
DADO* saida -> Saída da fila, ou seja, o elemento que se encontra em primeiro.
*/
typedef struct {

    DADO* entrada;
    DADO* saida;
} Fila;

typedef Fila* p_fila;

/*
Função que aloca espaço dinamicamente para uma fila.

Retorno:
p_fila -> ponteiro para uma estrutura Fila.
*/
p_fila criar_fila() {

    p_fila f = malloc(sizeof(Fila));
    f->entrada = NULL;
    f->saida = NULL;
    return f;
}

/*
Função booleana para verificar se uma fila é vazia.
Retorna 1 se a fila está vazia e 0 caso contrário.

Parâmetros:
p_fila f -> ponteiro de fila
*/
int fila_vazia(p_fila f){

    return (f->saida == NULL);
}

/*
Função responsável por liberar todo o espaço dentro da fila. Sem liberar o espaço alocado para a fila em si.

Parâmetros:
p_fila f -> ponteiro de fila
*/
void esvaziar_fila(p_fila f){

    DADO* aux;
    while(!fila_vazia(f)) {
        aux = f->saida;
        f->saida = f->saida->proximo;
        free(aux);
    }
    f->entrada = NULL;
}

/*
Função que além de esvaziar a fila libera o espaço alocado para o ponteiro da fila em si.

Parâmetros:
p_fila f -> ponteiro para a fila
*/
void liberar_fila(p_fila f){

    DADO* aux;
    while(!fila_vazia(f)) {
        aux = f->saida;
        f->saida = f->saida->proximo;
        free(aux);
    }
    free(f);
}

/*
Função para adicionar algo na fila.

Parâmetros:
p_fila f -> ponteiro da fila
int k -> dado a ser adicionado na fila

Retorno:
Vazio
*/
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

/*
Função responsável por remover o elemento em primeiro lugar na fila.

Parâmetros: 
p_fila f -> ponteiro para a fila para remover o dado.

Retorno:
retorna o dado removido caso exista dado. Caso contrário retorna INT_MIN.
*/
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

/*
Estima a quantidade de vértices(nós) existentes em uma árvore.

Parâmetros:
int qtd_filhos -> quantidade de filhos na árvore.
float prob -> probabilidade associada a cada nó.
int profundidade -> profundidade da árvore.

Retorno: 
número estimado de vértices arredondado para cima.
*/
int estimar_nos(int qtd_filhos, float prob, int profundidade) {
    
    int qtd_vertices = 0;
    float constante = qtd_filhos * prob;
    
    if (constante < 1)
        return ceil(1 / (1 - constante));


    for (int x = 0; x<=profundidade; x++) {
        qtd_vertices += pow(qtd_filhos, x*(x+1)/2);
    }
    return ceil(qtd_vertices);
}

/*
Inicializa a arvore vetorizada:

Parâmetros: 
int qtd_filhos -> quantidade de filhos por vértice(nó).
float prob -> probabilidade de cada filho existir.
int profundidade -> profundidade da árvore.

Retorno:
ponteiro para árvore inicializada.
*/
int* inicializar_arvore(int qtd_filhos, float prob, int profundidade) {

    int* arvore = (int*) malloc(((qtd_filhos+1)* estimar_nos(qtd_filhos, prob, profundidade) + 1) * sizeof(int));
    arvore[0] = qtd_filhos;
    return arvore;
}

/*
Libera a memória alocada para a árvore.

Parâmetros:
int* arvore -> ponteiro para a árvore a ser liberada
*/
void liberar_arvore(int* arvore) {
    free(arvore);
}

// TODO: Averiguar a necessidade de implementar função para esvaziar a árvore.

/*
Cria uma camada de abstração, para que o usuario nao se preocupe com o manejo dos índices do vetor.

Parâmetros:
valor_real -> valor original da posicao do vetor.
qtd_filhos -> quantidade de filhos que um vertice pode ter.

Retorno:
inteiro abstraído.

*/
int abstracao(int valor_real, int qtd_filhos) {
    if (valor_real==0)
        return 0;
    int valor_abstrato = 1 + (valor_real-1)/(qtd_filhos+1);
    return valor_abstrato;
}

/*
Desfaz a camada de abstracao, convertendo para o valor real na posicao do vetor

Parâmetros:
valor_abstrato -> valor abstraido da posicao dos vertices no vetor
qtd_filhos -> quantidade de filhos que um vertice pode ter

Retorno:
valor inteiro após defazer a abstração.
*/
int concretizacao(int valor_abstrato, int qtd_filhos) {
    int valor_real = 1 + (valor_abstrato-1)*(qtd_filhos+1);
    return valor_real;
}

/*
Retorna o pai do vertice inserido (ja abstraido)

Parâmetros:
arvore -> vetor de inteiros que estamos usando para trabalhar com uma arvore
vertice -> valor abstraido da posicao real dos dados do vertice no vetor
*/
int pai(int* arvore, int vertice) {
    int valor_real = concretizacao(vertice, arvore[0]);
    return abstracao(arvore[valor_real],arvore[0]);
}

/*
Retorna o filho do vertice inserido (ja abstraido)

Parâmetros:
arvore -> vetor de inteiros que estamos usando para trabalhar com uma arvore
vertice -> valor abstraido da posicao real dos dados do vertice no vetor
filho -> numero do filho em questao
*/
int filho(int* arvore, int vertice, int filho) {
    int valor_real = concretizacao(vertice, arvore[0]);
    return abstracao(arvore[valor_real + filho],arvore[0]);
}


// BIG RYAN NOTES 2.0
// TODO: adicionar verificação de caso o int* arvore não possua memória alocada, alocar memória.
int gerar_arvore(int* arvore, float prob, int profundidade) {

    int qtd_noh = 1;
    int qtd_filhos_max = arvore[0];
    int max_noh_possivel = estimar_nos(arvore[0], prob, profundidade);

    for(int i = 1; i<max_noh_possivel; i++) {
        arvore[i] = 0;
    }

    arvore[1] = 1; 
    // printf("prob do filho: %.2f\n", prob);
    // printf("|Pai %d", arvore[1]);

    for(int indice = 0, i = 2; i < max_noh_possivel; i++){

        if (i == (i/(qtd_filhos_max+1))*(qtd_filhos_max+1) + 1){
            if (arvore[i] == 0){
                // printf("\nFim da linhagem!");
                break;
            }
            // printf("\n|Pai %d", i);
            continue;
        }

        srand(time(NULL));

        if (rand() % 100 < (int)(prob*100) ? true : false){

            indice = 1 + ((qtd_filhos_max + 1) * qtd_noh);
            arvore[i] = indice;
            arvore[indice] = (i/(qtd_filhos_max+1))*(qtd_filhos_max+1) + 1;
            qtd_noh++;
        }

        // printf("|%d", arvore[i]);
    }

    return qtd_noh;
}
//Isso é um easter egg! +10 pontos //(i/(qtd_filhos+1))*(qtd_filhos+1) + 1
// i == 1 + ((qtd_filhos_max + 1) * qtd_noh)


#pragma endregion arvore

#pragma region matrizes

/*
Estrutura genérica de matriz. Criada com o intuito de construir um grafo.

Objetos da estrutura:
int n -> linhas.
int ** matriz -> ponteiro para matriz.
*/
typedef struct matricial {

    int n;          //linhas
    int **matriz; //ponteiro para matriz
}Matriz;


/*
Função que inicializa uma matriz para grafos.

Parâmetros:
int qtd_vertices -> quantidade de vértices desejados.

Retorno 
ponteiro para a matriz inicializada.
*/
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

/*
Função que libera o espaço dinamicamente alocado para uma matriz, incluindo todos os dados armazenados nela.

Parâmetros:
Matriz* matricial -> ponteiro para a matriz a ser liberada.
*/
void liberar_matriz(Matriz* matricial){

    for (int i = 0; i < matricial->n; i++) {
        free(matricial->matriz[i]);
    }
    free(matricial->matriz);
    free(matricial);
}
#pragma endregion matrizes

#pragma region grafo_aleatorio
/*
Função para gerar um grafo com arestas aleatórias.

Parâmetros:
Matriz* matricial -> ponteiro de matriz para se gerar as arestas.
bool orientado -> booleano que informa se a matriz é orientada ou não.
int probabilidade -> probabilidade de cada aresta.
*/
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

/*
Função de sobrecarga da busca em largura recursiva. 

Parâmetros:
int* arvore -> árvore para busca em largura.
int no_atual -> nó atual, índice.
int profundidade -> profundidade atual.
int no_anterior -> índice para o nó anterior.

Retorno
inteiro que representa a profundidade.
*/
int busca_em_largura_recursiva(int* arvore, int no_atual, int profundidade, int no_anterior) {

    int maior_global = profundidade;
    int maior_local = 0;

    for (int i=1; i<= arvore[0]; i++) {

        if(filho(arvore,no_atual,i) != 0 && filho(arvore,no_atual,i) != no_anterior) {

            maior_local = busca_em_largura_recursiva(arvore, filho(arvore,no_atual,i), maior_global + 1, no_atual);
            if (maior_global<maior_local)
                maior_global = maior_local;
        }
    }

    if (pai(arvore, no_atual) != no_atual) {

        maior_local = busca_em_largura_recursiva(arvore, pai(arvore, no_atual), maior_global + 1, no_atual);

        if (maior_global<maior_local)
            maior_global = maior_local;
    }

    if(maior_global > profundidade)
        return maior_global;
    return profundidade;
}

/*
Função que realiza recursivamente a busca em largura da árvore

Parâmetros:
int* arvore -> vetor da árvore para busca em largura.
int no_atual -> nó atual para busca em largura (índice) .

Retorno:
inteiro que representa a profundidade.
*/
int busca_em_largura(int* arvore, int no_atual) {
    return busca_em_largura_recursiva(arvore, no_atual, 0, 0);
}

/*
Função que obtém a média da profundidade de uma árvore.

Parâmetros: 
int* arvore -> vetor da árvore a ser calculada.
int qtd_noh -> quantidade de nós (tamanho do vetor de árvore).

Retorno: 
float que representa a média da profundidade.
*/
float media_busca_em_largura(int* arvore, int qtd_noh) {

    int soma = 0;
    for (int i = 1; i<=qtd_noh; i++) {
        soma += busca_em_largura(arvore, i);
    }
    return (1.0 * soma)/(qtd_noh * 1.0);
}

/*
Função que cria diferentes árvores e obtém a média geral da profundidade delas. A quantidade de árvores criadas é baseada em um typdef ITERACOES.

Parâmetros
int qtd_filhos -> quantidade de filhos na árvore
float prob -> probabilidade de cada nó
int profundidade -> profundidade da árvore

Retorno
float que representa a média geral da profundidade na busca em largura das árvores geradas.
*/
float media_geral_parametros(int qtd_filhos, float prob, int profundidade) {

    int* arvore = inicializar_arvore(qtd_filhos, prob, profundidade);
    float media_geral = 0.0;

    for (int i = 0; i<ITERACOES; i++) {

        media_geral += media_busca_em_largura(arvore, gerar_arvore(arvore, prob, profundidade));
    }

    liberar_arvore(arvore);
    return media_geral/ITERACOES;
}

/*
Função que solicita ao usuário o preenchimento de certas informações via prompt

Parâmetros:
int *qtd_filhos -> ponteiro para se armazenar a quantidade de filhos na árvore.
float *prob -> ponteiro para se armazenar a probabilidade de cada filho.
int *profundidade -> ponteiro para se armazenar a profundidade da árvore.
*/
void solicitar_ao_usuario(int *qtd_filhos, float *prob, int *profundidade) {

    do{
        printf("\nDigite a quantidade de filhos na arvore: ");
        scanf(" %d", qtd_filhos);
        if((*qtd_filhos) > 8 || (*qtd_filhos) < 1) {
            printf("\nDigite uma quantidade de filhos valida.\n");
        }
        
    }while((*qtd_filhos > 8 || *qtd_filhos < 1));

    do{

        printf("\nDigite a probabilidade da existencia dos filhos: ");
        scanf(" %f", prob);

        if((*prob) < 0 || (*prob) > 1){
            printf("\nDigite uma probabilidade valida\n");
        }
        
    }while((*prob < 0 || *prob > 1));

    do{

        printf("\nDigite qual a profundidade maxima da arvore: ");
        scanf(" %d", profundidade);

        if((*profundidade) < 0 || (*profundidade) > 4) {
            printf("\nDigite uma profundidade valida.\n");
        }

    }while((*profundidade < 0 || *profundidade > 4));
}

int main() {

    int qtd_filhos, profundidade;
    float prob;
    solicitar_ao_usuario(&qtd_filhos, &prob, &profundidade);

    int* arvore_testes = inicializar_arvore(qtd_filhos, prob, profundidade);
    int qtd_nos = gerar_arvore(arvore_testes, prob, profundidade);

    float media_profundidade_busca_em_largura = media_busca_em_largura(arvore_testes, qtd_nos);

    printf("TESTE");
    printf("A profundidade média da quantidade de nós é: %.2f", media_profundidade_busca_em_largura);

    return 0;
}


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
