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

            maior_local = busca_em_largura_recursiva(arvore, filho(arvore,no_atual,i), profundidade + 1, no_atual);
            if (maior_global<maior_local)
                maior_global = maior_local;
        }
    }

    if (pai(arvore, no_atual) != no_atual && pai(arvore, no_atual) != no_anterior && pai(arvore, no_atual) != 0) {

        maior_local = busca_em_largura_recursiva(arvore, pai(arvore, no_atual), profundidade + 1, no_atual);

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

int menu() {
    int opcao;

    do {
        printf("\n\n+-------------------Menu---------------------+\n|1 - Inserir dados                           |\n|2 - Carregar dados                          |\n|3 - Ver média de profundidade dos parâmetros|\n|4 - Sair                                    |\n+--------------------------------------------+\n");
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
                printf("\nA média geral dos parâmetros é: %.2f", media_geral_parametros(qtd_filhos, prob, profundidade));
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
