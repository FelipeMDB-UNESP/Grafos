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

#define ITERACOES 10

#pragma region fila

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

#pragma endregion fila

#pragma region log
int contador_logs = 1;

FILE* abrir_log() {
    char buffer[32];
    sprintf(buffer, "log%d.txt", contador_logs++);
    FILE* log = fopen(buffer,"w");
    return log;
}

void fechar_log(FILE* log) {
    fclose(log);
}

void cabecalho_log(FILE* log, int max_noh_possivel, int qtd_filhos, float prob) {

    char buffer[32];
    int margem;

    fprintf(log,"+-------------------------+\n");

    fprintf(log,"|Espaço Alocado: %d", max_noh_possivel);

    sprintf(buffer, "%d", max_noh_possivel);
    margem = strlen("-------------------------") -strlen("Espaço Alocado: ") -strlen(buffer);
    for (int i=0; i<=margem; i++) {
        fputc(' ', log);
    }
    fputc('|', log);
    fputc('\n', log);


    fprintf(log,"|qtd de filhos: %d", qtd_filhos);

    sprintf(buffer, "%d", qtd_filhos);
    margem = strlen("-------------------------") -strlen("qtd de filhos: ") -strlen(buffer);
    for (int i=0; i<margem; i++) {
        fputc(' ', log);
    }
    fputc('|', log);
    fputc('\n', log);

    fprintf(log,"|prob do filho: %.2f", prob);

    sprintf(buffer, "%.2f", prob);
    margem = strlen("-------------------------") -strlen("prob do filho: ") -strlen(buffer);
    for (int i=0; i<margem; i++) {
        fputc(' ', log);
    }
    fputc('|', log);
    fputc('\n', log);

    fprintf(log,"+-------------------------+\n");
}
#pragma endregion log

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

    int* arvore = (int*) calloc(((qtd_filhos+1)* estimar_nos(qtd_filhos, prob, profundidade) + 1), sizeof(int));
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
    int qtd_filhos = arvore[0];
    int camada = 0;
    int ultimo_gerado = 0;
    bool fim_geracao;
    int max_noh_possivel = estimar_nos(arvore[0], prob, profundidade);

    for(int i = 1; i<((qtd_filhos+1)* max_noh_possivel + 1); i++) {
         arvore[i] = 0;
    }
    arvore[1] = 1;

    FILE* log = abrir_log();
    cabecalho_log(log, max_noh_possivel, arvore[0], prob);
    fprintf(log,"|Pai %d", arvore[1]);

    srand(time(NULL));
    for(int indice = 0, i = 2; camada <= profundidade; i++){

        if (i == (i/(qtd_filhos+1))*(qtd_filhos+1) + 1){
            if (arvore[i] == 0){
                fprintf(log,"\nFim da linhagem!");
                break;
            }

            if (camada == 0 || fim_geracao) {
                camada++;
                ultimo_gerado = indice;
                fim_geracao = false;
            }
            if (ultimo_gerado == i)
                fim_geracao = true;
            if (camada <= profundidade)
                fprintf(log,"\n|Pai %d", i);
            continue;
        }

        if (camada < profundidade) {
            if (rand() % 100 < (int)(prob*100) ? true : false){

                indice = 1 + ((qtd_filhos + 1) * qtd_noh);
                arvore[i] = indice;
                arvore[indice] = (i/(qtd_filhos+1))*(qtd_filhos+1) + 1;
                qtd_noh++;
            }
        }

        fprintf(log,"|%d", arvore[i]);
    }

    fechar_log(log);
    return qtd_noh;
}

#pragma endregion arvore



#pragma region bfs_iterativa
int bfs_arvore_binomial (int *arvore, int raiz, int qtd_nos) {

    // Vetor para armazenar as profundidades de cada nó
    float *profundidades = (float *) malloc(qtd_nos * sizeof(float));
    //precisávamos iterar em profundidades de forma sequencial, essa é a maneira mais simples;
    int pos_profundidades = 0;
    //Marca os nós visitados. Faz-se necessário pois temos de lidar com a possibilidade da raiz não ser mais o primeiro nó
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
        visitados[no_atual / bloco] = 1;
        // printf("Visitando no: %d\n", (no_atual / bloco) + 1); // Visita o nó atual

        //i começa em 0 para podermos usar essa variável para encontrar os filhos e pais na posição correta.
        //no_atual - 1 -> para posicionarmos no começo das informações do nó, não em seu fim.
        //divide-se por bloco pois cada bloco representa um nó de forma abstrata. A divisão fornece o nó verdadeiro.
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

float media_bfs_iterativa (int *arvore, int raiz, int qtd_nos, int bloco) {

    float soma_profundidades = 0;
    // Chama a função para cada nó como raiz
    //quantidade de filhos + 1 é como se fôssemos de índice a índice.
    // no meio do for é bloco - 1
    printf("\nNOVA ÁRVORE:\n");
    for (int raiz = 1; raiz <= ((qtd_nos-1) * bloco) + 1; raiz  = raiz + bloco) {
        int profundidade = bfs_arvore_binomial(arvore, raiz, qtd_nos);
        printf("Profundidade da arvore com raiz = noh %d: %d\n", (raiz/bloco)+1, profundidade);
        soma_profundidades += (float)profundidade; // Acumula a soma das profundidades
    }

    return soma_profundidades / qtd_nos;
}

/*
Função que com os parâmetros fornecidos gera árvore 10x e executa bfs para obter a média da profundidade.
*/
float media_geral_parametros(int qtd_filhos, float prob, int profundidade) {

    int* arvore = inicializar_arvore(qtd_filhos, prob, profundidade);
    float media_geral = 0.0;

    for (int i = 0; i<ITERACOES; i++) {
        int qtd_nos = gerar_arvore(arvore, prob, profundidade);
        media_geral += media_bfs_iterativa(arvore, 1, qtd_nos, qtd_filhos+1);
    }

    liberar_arvore(arvore);
    return media_geral/ITERACOES;
}
#pragma endregion bfs_iterativa

/*
Função que solicita ao usuário o preenchimento de certas informações via prompt

Parâmetros:
int *qtd_filhos -> ponteiro para se armazenar a quantidade de filhos na árvore.
float *prob -> ponteiro para se armazenar a probabilidade de cada filho.
int *profundidade -> ponteiro para se armazenar a profundidade da árvore.
*/
void solicitar_ao_usuario(int *qtd_filhos, float *prob, int *profundidade) {

    printf("Os valores estão limitados para garantir a disponibilidade de memória e velocidade.");
    do{
        printf("\nDigite a quantidade de filhos na arvore (entre 1 e 8): ");
        scanf(" %d", qtd_filhos);
        if((*qtd_filhos) > 8 || (*qtd_filhos) < 1) {
            printf("\nDigite uma quantidade de filhos valida.\n");
        }
        
    }while((*qtd_filhos > 8 || *qtd_filhos < 1));

    do{

        printf("\nDigite a probabilidade da existencia dos filhos (entre 0 e 1): ");
        scanf(" %f", prob);

        if((*prob) < 0 || (*prob) > 1){
            printf("\nDigite uma probabilidade valida\n");
        }
        
    }while((*prob < 0 || *prob > 1));

    do{

        printf("\nDigite qual a profundidade maxima da arvore (entre 0 e 4): ");
        scanf(" %d", profundidade);

        if((*profundidade) < 0 || (*profundidade) > 4) {
            printf("\nDigite uma profundidade valida.\n");
        }

    }while((*profundidade < 0 || *profundidade > 4));
}

/*
Função para que o usuário forneça um arquivo para criar a árvore.
*/
void solicitar_ao_arquivo(int *qtd_filhos, float *prob, int *profundidade) {

    char buffer[16];

    printf("\nDigite o nome do arquivo texto a extrair os dados:\n");
    scanf(" %s",buffer);

    FILE* teste = fopen(buffer,"r+");

    fgets(buffer, 16, teste);
    *qtd_filhos = atoi(buffer);

    fgets(buffer, 16, teste);
    *prob = atof(buffer);

    fgets(buffer, 16, teste);
    *profundidade = atoi(buffer);

    fclose(teste);
}

/*
Cria um meno interativo para o usuário com 4 opções.
1 -> Inserir dados
2 -> Carregar dados
3 -> BFS
4 -> Sair
*/
int menu() {
    int opcao;

    do {
        printf("\n\n+-------------------Menu---------------------+\n|1 - Inserir dados                           |\n|2 - Carregar dados                          |\n|3 - Ver media de profundidade dos parametros|\n|4 - Sair                                    |\n+--------------------------------------------+\n");
        scanf(" %d", &opcao);
    } while (opcao <1 || opcao > 4);

    return opcao;
}

int main() {

    int qtd_filhos = -1;
    int profundidade = -1;
    float prob = -1.0;
    int caso;

    while (true) {

        caso = menu();

        switch (caso) {
        case 1:
            solicitar_ao_usuario(&qtd_filhos, &prob, &profundidade);
            break;
        case 2:
            solicitar_ao_arquivo(&qtd_filhos, &prob, &profundidade);
            break;
        case 3:
            if (qtd_filhos != -1) {
                printf("\nA media geral dos parametros eh: %f", media_geral_parametros(qtd_filhos, prob, profundidade));
            } else {
                printf("\nAdicione dados primeiro.\n");
            }
            break;
        case 4:
            exit(0);
            break;
        }
    }
    return 0;
}
