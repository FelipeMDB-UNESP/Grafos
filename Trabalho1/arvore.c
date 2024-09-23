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
    for (int i = 1; i<((qtd_filhos+1)* estimar_nos(qtd_filhos, prob, profundidade) + 1); i++)
        arvore[i] = 0;
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
int* gerar_arvore(int* arvore, int qtd_filhos_max, int prob_filho, int profundidade, int max_noh_possivel){
    srand(time(NULL));
    int i = 1;
    int j = 1;
    int qtd_noh = 1;
    bool criou = false;
    int indice;
    arvore[i] = 1; 
    printf("prob do filho: %d\n", prob_filho);
    printf("|Pai %d", arvore[i]);
    for(i = 2; i < max_noh_possivel; i++){
        // if (i == 1 + ((qtd_filhos_max + 1) * qtd_noh)){
        if (i == (i/(qtd_filhos_max+1))*(qtd_filhos_max+1) + 1){
            if (arvore[i] == 0){
                printf("\nFim da linhagem!");
                break;
            }
            j = j + 5;
            printf("\n|Pai %d", i);
            continue;
        }
        int range_teste = rand();
        criou = rand() % 100 < prob_filho ? true : false;
        if (criou){
            indice = 1 + ((qtd_filhos_max + 1) * qtd_noh);
            arvore[i] = indice;
            arvore[indice] = j;
            qtd_noh++;
            
        }
        printf("|%d", arvore[i]);
        j++;
    }
    //Isso é um easter egg! +10 pontos //(i/(qtd_filhos+1))*(qtd_filhos+1) + 1
}


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
Função que solicita ao usuário a inserção de um valor para uma probabilidade (por linhas de comando). O valor deve ser um número entre 0 e 1

Retorno:
retorna um float para a probabilidade inserida pelo usuário.
*/
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

/*
Função para realizar BFS e calcular a média das profundidades.

Parâmetros:
int* arvore -> ponteiro para a árvore 
int tamanho -> tamanho do vetor de árvore, ou seja, quantos nós existem na árvore.
*/
double bfs_media_profundidade(int* arvore, int tamanho) {
    if (tamanho == 0) 
        return 0.0;

    int profundidade = 0;
    int num_nos = 0;
    double soma_profundidades = 0.0;

    // Fila para BFS usando vetor
    int* fila = (int*)malloc(tamanho * sizeof(int));
    int saida = 0, entrada = 0;

    // Começa com a raiz (índice 0)
    fila[entrada++] = 0;

    while (saida < entrada) {
        int nivel_atual = entrada - saida; // Número de nós no nível atual

        // Processa todos os nós do nível atual
        for (int i = 0; i < nivel_atual; i++) {
            int indice_atual = fila[saida++];

            soma_profundidades += profundidade;
            num_nos++;

            // Adiciona os filhos à fila
            for (int j = 1; j <= arvore[indice_atual] && indice_atual + j < tamanho; j++) {
                fila[entrada++] = indice_atual + j; // A posição dos filhos
            }
        }
        profundidade++; // Aumenta a profundidade ao passar para o próximo nível
    }

    free(fila); // Libera a memória da fila

    // Calcula a média das profundidades
    return soma_profundidades / num_nos;
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

    int* arvore_testes = inicializar_arvore(4, 0.32, 4);
    gerar_arvore(arvore_testes, 4, 32, 4, 100); // USANDO 100 TEMPORÁRIAMENTE!

    return 0;
}