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
#include <string.h>

#pragma region vetores
/*
Função responsável por criar dinamincamente um vetor.

Parâmetros:
int n -> tamanho do vetor

Retorno:
int* vetor -> vetor dinamicamente alocado
*/
int* criar_vetor(int n) {

    int* vetor = (int*)calloc(n, sizeof(int));
    return vetor;
}

/*
Função responsável por trocar as posições de um vetor.

Parâmetros:
int* vetor -> ponteiro do vetor a ser manipulado
int num1 -> indice de uma das posições do vetor
int num2 -> indice de uma das posições do vetor

Retorno: void
*/
void troca_lugares(int* vetor, int num1, int num2) {
    int aux = vetor[num1];
    vetor[num1] = vetor[num2];
    vetor[num2] = aux;
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

bool remover_item(p_fila f, int k) {

    DADO* atual = f->saida;
    DADO* anterior = NULL;

    while (atual != NULL) {

        if (atual->dado == k) {
            
            if (anterior != NULL){
                
                anterior->proximo = atual->proximo;
                free(atual);

            } else {
                desenfileirar(f);
            }

            return true;
        }

        anterior = atual;
        atual = atual->proximo;
    }
    return false;
}
#pragma endregion filas

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
    int *grau;   //ponteiro para vetor com o grau dos vértices
    p_fila *lista_n_adjascencia; //ponteiro para vetor da lista de não adjascência
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

    matricial->lista_n_adjascencia = malloc(sizeof(p_fila) * qtd_vertices);
    for (int i = 0; i<qtd_vertices; i++) {
        matricial->lista_n_adjascencia[i] = criar_fila();
    }

    matricial->grau = criar_vetor(qtd_vertices);
    return matricial;
}

Matriz* copiar_matriz(Matriz* matricial) {

    Matriz* copia = inicializar_matriz(matricial->n);
    DADO* aux;

    for(int i = 0; i< copia->n; i++) {

        copia->grau[i] = matricial->grau[i];

        aux = matricial->lista_n_adjascencia[i]->saida;
        while(aux !=NULL) {
            enfileirar(copia->lista_n_adjascencia[i], aux->dado);
            aux = aux->proximo;
        }

        for(int j = 0; j< copia->n; j++) {
            copia->matriz[i][j] = matricial->matriz[i][j];
        }
    }

    return copia;
}

/*
Função que libera o espaço dinamicamente alocado para uma matriz, incluindo todos os dados armazenados nela.

Parâmetros:
Matriz* matricial -> ponteiro para a matriz a ser liberada.
*/
void liberar_matriz(Matriz* matricial){

    for (int i = 0; i < matricial->n; i++) {
        free(matricial->matriz[i]);
        liberar_fila(matricial->lista_n_adjascencia[i]);
    }
    
    liberar_vetor(matricial->grau);
    free(matricial->lista_n_adjascencia);
    free(matricial->matriz);
    free(matricial);
}
#pragma endregion matrizes

#pragma region grafo

/*Atribuição de nome para  facilitar o entendimento do grafo*/
typedef Matriz* Grafo;

/*
Função para gerar um grafo com arestas aleatórias.

Parâmetros:
Matriz* matricial -> ponteiro de matriz para se gerar as arestas.
bool orientado -> booleano que informa se a matriz é orientada ou não.
int probabilidade -> probabilidade de cada aresta.
*/
void gerar_grafo(Grafo grafo, bool orientado, float probabilidade) {
    
    int porcentagem = (int) (100*probabilidade);
    srand(time(NULL));

    if (!orientado) {   //garante espelhamento 
        for (int i = 0; i < grafo->n; i++) {
            for (int j = i; j < grafo->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    grafo->matriz[i][j] = (rand() % 100 < porcentagem) ? (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)
                    grafo->matriz[j][i] = grafo->matriz[i][j];

                    if (grafo->matriz[i][j]) {
                        grafo->grau[i] ++;
                        grafo->grau[j] ++;

                    } else {
                        enfileirar(grafo->lista_n_adjascencia[i],j);
                        enfileirar(grafo->lista_n_adjascencia[j],i);
                    }
                    
                } else {
                    grafo->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    } else {
        for (int i = 0; i < grafo->n; i++) {
            for (int j = 0; j < grafo->n; j++) {
                if (i != j) {                          //evitar ligacoes proprias
                    grafo->matriz[i][j] = (rand() % 100 < porcentagem) ? (rand() % 10 + 1) : 0;//random de números entre 0 e 99 (resto da divisao por 100)

                    if (grafo->matriz[i][j]) {
                        grafo->grau[i] ++;
                    } else {
                        enfileirar(grafo->lista_n_adjascencia[i],j);
                    }

                } else {
                    grafo->matriz[i][j] = 0;       //falso para quando for a diagonal principal       
                }
            }
        }
    }
}

void gerar_grafo_hamiltoniano(Grafo grafo, bool orientado, float probabilidade) {

    gerar_grafo(grafo, orientado, probabilidade);

    srand(time(NULL));
    int* ciclo = criar_vetor(grafo->n);

    for(int i = 0; i<grafo->n; i++) {
        ciclo[i]=i;
    }

    if(!orientado) {

        troca_lugares(ciclo, 0, rand()%(grafo->n));

        for(int i = 1; i<grafo->n; i++) {
            
            troca_lugares(ciclo, i, rand()%(grafo->n - i) + i);

            if (!(grafo->matriz[ciclo[i-1]][ciclo[i]])) {

                grafo->matriz[ciclo[i-1]][ciclo[i]] = rand()%10+1;
                grafo->matriz[ciclo[i]][ciclo[i-1]] = grafo->matriz[ciclo[i-1]][ciclo[i]];
                grafo->grau[ciclo[i-1]] ++;
                grafo->grau[ciclo[i]] ++;
                remover_item(grafo->lista_n_adjascencia[ciclo[i-1]], ciclo[i]);
                remover_item(grafo->lista_n_adjascencia[ciclo[i]], ciclo[i-1]);
            }
        }

        if (!(grafo->matriz[ciclo[grafo->n -1]][ciclo[0]])) {

            grafo->matriz[ciclo[grafo->n -1]][ciclo[0]] = rand()%10+1;
            grafo->matriz[ciclo[0]][ciclo[grafo->n -1]] = grafo->matriz[ciclo[grafo->n -1]][ciclo[0]];
            grafo->grau[ciclo[grafo->n -1]] ++;
            grafo->grau[ciclo[0]] ++;
            remover_item(grafo->lista_n_adjascencia[ciclo[grafo->n -1]], ciclo[0]);
            remover_item(grafo->lista_n_adjascencia[ciclo[0]], ciclo[grafo->n -1]);
        }

    }
    liberar_vetor(ciclo);
}

bool Dirac(Grafo grafo) {
    
    for (int i = 0; i< grafo->n; i++) {
        if (grafo->grau[i] < grafo->n / 2)
            return false;
    }
    return true;
}

bool Ore(Grafo grafo) {

    DADO* aux;

    for (int i = 0; i<grafo->n; i++) {

        aux = grafo->lista_n_adjascencia[i]->saida;

        while(aux!=NULL) {
            if (grafo->grau[i] + grafo->grau[aux->dado] < grafo->n)
                return false;
            aux = aux->proximo;
        }
    }
    return true;
}

Grafo Fecho_Hamiltoniano(Grafo grafo) {

    Grafo fecho_hamiltoniano = copiar_matriz(grafo);
    srand(time(NULL));
    int aux;

    for (int i = 0; i<fecho_hamiltoniano->n; i++) {

        
        while (fecho_hamiltoniano->lista_n_adjascencia[i]->saida != NULL) {

            aux = desenfileirar(fecho_hamiltoniano->lista_n_adjascencia[i]);
            fecho_hamiltoniano->matriz[i][aux] = rand()%10+1;
            fecho_hamiltoniano->matriz[aux][i] = fecho_hamiltoniano->matriz[i][aux];
            fecho_hamiltoniano->grau[i]++;
            fecho_hamiltoniano->grau[aux]++;

            if(Ore(fecho_hamiltoniano))
                return fecho_hamiltoniano;
        }
    }
    liberar_matriz(fecho_hamiltoniano);
    return NULL;
}

bool Bondy_Chvatl(Grafo grafo) {

    Grafo fecho_hamiltoniano = Fecho_Hamiltoniano(grafo);

    if (fecho_hamiltoniano == NULL)
        return false;

    for (int i = 0; i<grafo->n; i++) {
        for (int j = i+1; j<grafo->n; i++) {

            if(! (fecho_hamiltoniano->matriz[i][j])) {
                liberar_matriz(fecho_hamiltoniano);
                return false;
            }
        }
    }
    liberar_matriz(fecho_hamiltoniano);
    return true;
}

/*
Função para visualizar a estrutura do grafo.

Parâmetros:
Grafo grafo -> ponteiro para o grafo a ser visualizado.
bool salvar_em_arquivo -> booleano que indica se deve salvar em arquivo ou imprimir no console.
*/
void visualizar_grafo(Grafo grafo, bool salvar_em_arquivo) {
    FILE *file = NULL;
    if (salvar_em_arquivo) {
        file = fopen("grafo.txt", "w");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo para escrita.\n");
            return;
        }
    }

    FILE *output = salvar_em_arquivo ? file : stdout;

    if (grafo == NULL) {
        fprintf(output, "Grafo nao existe.\n");
        if (salvar_em_arquivo) fclose(file);
        return;
    }

    fprintf(output, "Matriz de Adjacencia:\n");
    for (int i = 0; i < grafo->n; i++) {
        for (int j = 0; j < grafo->n; j++) {
            fprintf(output, "%d ", grafo->matriz[i][j]);
        }
        fprintf(output, "\n");
    }

    fprintf(output, "\nGraus dos vertices:\n");
    for (int i = 0; i < grafo->n; i++) {
        fprintf(output, "Vértice %d: %d\n", i, grafo->grau[i]);
    }

    fprintf(output, "\nListas de Nao Adjacencia:\n");
    for (int i = 0; i < grafo->n; i++) {
        fprintf(output, "Vértice %d: ", i);
        DADO* aux = grafo->lista_n_adjascencia[i]->saida;
        while (aux != NULL) {
            fprintf(output, "%d ", aux->dado);
            aux = aux->proximo;
        }
        fprintf(output, "\n");
    }

    if (salvar_em_arquivo) fclose(file);
}

int main() {
    int opcao;
    Grafo grafo = NULL;
    int n;
    float probabilidade;
    static int orientado = false;

    while (true) {
        printf("+-------------------Menu---------------------+\n");
        printf("Escolha uma opcao:\n");
        printf("1. Gerar Grafo\n");
        printf("2. Gerar Grafo Hamiltoniano\n");
        printf("3. Verificar Teorema de Dirac\n");
        printf("4. Verificar Teorema de Ore\n");
        printf("5. Verificar Fecho Hamiltoniano\n");
        printf("6. Verificar Bondy-Chvatal\n");
        printf("7. Visualizar Grafo\n");
        printf("0. Sair\n");
        printf("+--------------------------------------------+\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if(grafo) {
                    liberar_matriz(grafo);
                    printf("Grafo anterior existente foi excluido!\n");
                }
                printf("Digite o numero de vertices: ");
                scanf("%d", &n);
                printf("Digite a probabilidade de aresta (0 a 1): ");
                scanf("%f", &probabilidade);
                grafo = inicializar_matriz(n);
                gerar_grafo(grafo, orientado, probabilidade);
                printf("Grafo gerado com sucesso!\n");
                break;
            case 2:
                if(grafo) {
                    liberar_matriz(grafo);
                    printf("Grafo anterior existente foi excluido!\n");
                }
                printf("Digite o numero de vertices: ");
                scanf("%d", &n);
                printf("Digite a probabilidade de aresta (0 a 1): ");
                scanf("%f", &probabilidade);
                grafo = inicializar_matriz(n);
                gerar_grafo_hamiltoniano(grafo, orientado, probabilidade);
                printf("Grafo Hamiltoniano gerado com sucesso!\n");
                break;
            case 3:
                if(grafo == NULL) {
                    printf("Gere um grafo primeiro!\n");
                    break;
                }
                if (Dirac(grafo)) {
                    printf("O grafo satisfaz o Teorema de Dirac.\n");
                } else {
                    printf("O grafo nao satisfaz o Teorema de Dirac.\n");
                }
                break;
            case 4:
                if(grafo == NULL) {
                    printf("Gere um grafo primeiro!\n");
                    break;
                }
                if (Ore(grafo)) {
                    printf("O grafo satisfaz o Teorema de Ore.\n");
                } else {
                    printf("O grafo nao satisfaz o Teorema de Ore.\n");
                }
                break;
            case 5:
                if(grafo == NULL) {
                    printf("Gere um grafo primeiro!\n");
                    break;
                }
                if (Fecho_Hamiltoniano(grafo)) {
                    printf("O grafo possui Fecho Hamiltoniano.\n");
                } else {
                    printf("O grafo nao possui Fecho Hamiltoniano.\n");
                }
                break;
            case 6:
                if(grafo == NULL) {
                    printf("Gere um grafo primeiro!\n");
                    break;
                }
                if (Bondy_Chvatl(grafo)) {
                    printf("O grafo satisfaz o Teorema de Bondy-Chvatal.\n");
                } else {
                    printf("O grafo nao satisfaz o Teorema de Bondy-Chvatal.\n");
                }
                break;
            case 7:
                if(grafo == NULL) {
                    printf("Gere um grafo primeiro!\n");
                    break;
                }
                printf("Deseja salvar em arquivo? (1 - Sim, 0 - Nao): ");
                int salvar;
                scanf("%d", &salvar);
                visualizar_grafo(grafo, salvar);
                if (salvar) {
                    printf("Grafo salvo em 'grafo.txt'.\n");
                }
                break;
            case 0:
                liberar_matriz(grafo);
                return 0;
            default:
                printf("Opção invalida!\n");
                break;
        }
    }
}