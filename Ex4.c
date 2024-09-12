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
#include <time.h>

#pragma region structs

/*
carcaça para se utilizar na criação de grafos.
*/
typedef struct matriz {

    int n;          //linhas
    int **matriz; //ponteiro para matriz, no caso desse exercício é uma matriz de adjacências
} Matriz;

/*
estrutura de uma aresta.
v_origem -> vértice de origem da aresta.
v_destino -> vértice de destino da aresta.
peso -> peso da aresta.
*/
typedef struct aresta {
    int v_origem;
    int v_destino;
    int peso;
} Aresta;

/*
Árvore geradora mínima
bool *arestas -> vetor com TODAS as arestas do grafo mas as que pertencem à árvore estão marcadas como verdadeiro.
total_arestas -> total de arestas na árvore mínima.
peso_total -> soma dos pesos das arestas pertencentes.
*/
typedef struct Arvore {

    int * arestas;
    int qtd_arestas_arvore_minima;
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


#pragma endregion structs

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



/*função que gera um grafo com arestas aleatorias*/
void gerar_grafo(Matriz* matricial, bool orientado, int probabilidade) {

    if (!orientado) {   //garante espelhamento
        srand(time(NULL)); 
        for (int i = 0; i < matricial->n; i++) {

            for (int j = i; j < matricial->n; j++) {

                if (i != j) {   
                                           //evitar ligacoes proprias
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
                if (i != j) {     
                    //evitar ligacoes proprias
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

void liberar_vetor_arestas(Aresta * vetor) {

    free(vetor);
}
#pragma endregion vetores

#pragma region kruskal

int particiona(Aresta * arestas, int esq, int dir) {

    Aresta pivo = arestas[dir];
    int i = esq - 1;
    for (int j = esq; j < dir; j++) {

        if (arestas[j].peso < pivo.peso) {

            i++;
            Aresta temp = arestas[i];
            arestas[i] = arestas[j];
            arestas[j] = temp;
        }
    }
    Aresta temp = arestas[i + 1];
    arestas[i + 1] = arestas[dir];
    arestas[dir] = temp;
    return i + 1;
}

void quick_sort_arestas(Aresta * arestas, int esq, int dir) {
    if (esq < dir) {

        int pivo = particiona(arestas, esq, dir);
        quick_sort_arestas(arestas, esq, pivo - 1);
        quick_sort_arestas(arestas, pivo + 1, dir);
    }
}

/*
Encontrar o vértice que representa a compononente conexa do grafo.
pais -> vetor que cada posição é um dos vértices.
pos -> posição do "pai" da componente conexa.
*/
int pai_verdadeiro(int* pais, int pos) {

    int aux = pos;
    while (pais[aux] != aux) {
        
        aux = pais[aux];
    }
    return aux;
}

/*
Kruskal gera uma árvore de custo mínimo. Um caminho que passa por todos os vértices escolhendo as arestas de menor valor possível (sem formar ciclos).
arestas -> vetor de arestas do grafo.
qtd_arestas -> quantidade de arestas.
qtd_vertices -> quantidade de vértices.
*/
ArvoreGeradoraMinima kruskal(Aresta * arestas, int qtd_arestas, int qtd_vertices) {

    int peso_total = 0, quantidade_arestas = 0;
    int * arestas_registradas = (int*) calloc(qtd_arestas,sizeof(int));
    int * vertices_visitados = (int*) calloc(qtd_vertices,sizeof(int));
    int * pais_vertices = (int*) malloc(sizeof(int)*qtd_vertices);    

    quick_sort_arestas(arestas, 0, qtd_arestas-1);

    for(int aux, temp, i = 0; i < qtd_arestas; i++) {
        //checa se ambos foram visitados. Caso sim verifica seus pais verdadeiros para descobrir se forma componente conexa.
        if (vertices_visitados[arestas[i].v_origem] && vertices_visitados[arestas[i].v_destino]) {

            temp = pai_verdadeiro(pais_vertices,arestas[i].v_origem);
            aux = pai_verdadeiro(pais_vertices, arestas[i].v_destino);
            if (temp != aux) {
                
                pais_vertices[aux] = temp;
                arestas_registradas[i]=1;
                peso_total += arestas[i].peso;
                quantidade_arestas++;
            }
            continue;
        }

        //Caso ambos não foram visitados, ou seja, nova componente conexa.
        if (!vertices_visitados[arestas[i].v_origem] && !vertices_visitados[arestas[i].v_destino]) {

            pais_vertices[arestas[i].v_origem] = arestas[i].v_origem;
            pais_vertices[arestas[i].v_destino] = arestas[i].v_origem;
        } else {

            if (vertices_visitados[arestas[i].v_origem]) {

                pais_vertices[arestas[i].v_destino] = pais_vertices[arestas[i].v_origem];
            } else {

                pais_vertices[arestas[i].v_origem] = pais_vertices[arestas[i].v_destino];
            }
        }

        vertices_visitados[arestas[i].v_origem] = 1;
        vertices_visitados[arestas[i].v_destino] = 1;
        arestas_registradas[i]=1;
        peso_total += arestas[i].peso;
        quantidade_arestas++;
    }
    free(vertices_visitados);
    free(pais_vertices);
    ArvoreGeradoraMinima arvore_minima = {arestas_registradas, quantidade_arestas, peso_total};
    return arvore_minima;
}

/*Printa o resultado das arestas que compõem a árvore mínima.*/
void leitura_kruskal(Aresta * aresta, int qtd_arestas, ArvoreGeradoraMinima arvore) {

    printf("Quantidade de arestas na arvore minima: %d\nPeso total: %d\n", arvore.qtd_arestas_arvore_minima, arvore.peso_total);
    for(int i = 0; i < qtd_arestas; i++) {
        if (arvore.arestas[i])
            printf(" Origem: %d, Destino: %d, Peso: %d\n", aresta[i].v_origem, aresta[i].v_destino, aresta[i].peso);
    }
}

/*
Toda árvore mínima possui o número de arestas igual a (n° de vértices - 1).
Obviamente, para isso ser verdade, o grafo deve ser conexo.
*/
bool conexo(ArvoreGeradoraMinima arvore, int qtd_vertices) {

    if(arvore.qtd_arestas_arvore_minima != qtd_vertices - 1)
        return false;

    return true;
}

#pragma endregion kruskal

#pragma region testes
void teste_ordenacao_arestas() {

    Aresta a = {1, 2, 10};
    Aresta b = {2,3, 5};
    Aresta c = {3, 4, 2};
    Aresta d = {4, 1, 7};

    Aresta arestas[] = {a, b, c, d};
    quick_sort_arestas(arestas, 0, 3);

    for(int i = 0; i < 4; i++){
        printf(" %d \n", arestas[i].peso);
    }
}

#pragma endregion testes

Aresta *grafo_to_arestas(int qtd_vertices, Matriz *grafo, int *qtd_total_arestas) {

    Aresta *arestas = criar_vetor_arestas(qtd_vertices * (qtd_vertices-1));
    *qtd_total_arestas = 0;
    for(int i = 0; i < qtd_vertices; i++) {
        for(int j = 0; j < qtd_vertices; j++) {

            if(grafo->matriz[i][j]) {

                //printf("Peso: %d, Origem: %d, Destino: %d\n", grafo->matriz[i][j], i, j);
                arestas[*qtd_total_arestas].peso = grafo->matriz[i][j];
                arestas[*qtd_total_arestas].v_origem = i;
                arestas[*qtd_total_arestas].v_destino = j;
                *qtd_total_arestas++;
            }
        }
    }
    return arestas;
}

int main() {

    int qtd_vertices, origem, direcionado, prob;
    int qtd_total_arestas = 0;
    Aresta *arestas;
    ArvoreGeradoraMinima arvore;
    
    printf("Qual a quantidade de vertices?\n");
    scanf(" %d", &qtd_vertices);

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
        
        do {
            gerar_grafo(grafo, direcionado, (int)(probabilidade() * 100));

            arestas = grafo_to_arestas(qtd_vertices, grafo, &qtd_total_arestas);

            arvore = kruskal(arestas, qtd_total_arestas, qtd_vertices);
                
        } while (!conexo(arvore, qtd_vertices));
        
        leitura_kruskal(arestas, qtd_total_arestas, arvore);
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

        arestas = grafo_to_arestas(qtd_vertices, grafo, &qtd_total_arestas);

        }
        arvore = kruskal(arestas, qtd_total_arestas, qtd_vertices);
        if(conexo(arvore, qtd_vertices)) {

            leitura_kruskal(arestas, qtd_total_arestas, arvore);
        }
        else{

            printf("O grafo fornecido nao se trata de um grafo conexo");
        }
    }

    // Libera a memória utilizada
    liberar_matriz(grafo);
    liberar_vetor_arestas(arestas);
    free(arvore.arestas);

    return 0;
}
