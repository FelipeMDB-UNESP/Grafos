/*
Por:
André Luis Dias Nogueira
Felipe Melchior de Britto
Rafael Daiki Kaneko
Ryan Hideki Tadeo Guimarães
Vitor Marchini Rolisola
*/
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int contador_cliques; // utilizado para nomeação de arquivos

#pragma region matrizes

/*
Estrutura genérica de matriz. Criada com o intuito de construir um grafo.

Objetos da estrutura:
int n -> linhas.
int ** matriz -> ponteiro para matriz.
*/
typedef struct matricial {

  int n;        // linhas
  int **matriz; // ponteiro para matriz
} Matriz;

/*
Função que inicializa uma matriz para grafos.

Parâmetros:
int qtd_vertices -> quantidade de vértices desejados.

Retorno
ponteiro para a matriz inicializada.
*/
Matriz *inicializar_matriz(int qtd_vertices) {

  Matriz *matricial = (Matriz *)malloc(sizeof(Matriz));
  if (matricial == NULL) {
    printf("Falha na alocacao de memoria.\n");
    exit(1);
  }

  int **matriz_adjascencia = (int **)malloc(qtd_vertices * sizeof(int *));
  for (int i = 0; i < qtd_vertices; i++) {
    matriz_adjascencia[i] = (int *)malloc(qtd_vertices * sizeof(int));
  }
  matricial->n = qtd_vertices;
  matricial->matriz = matriz_adjascencia;
  return matricial;
}

/*
Função que libera o espaço dinamicamente alocado para uma matriz, incluindo
todos os dados armazenados nela.

Parâmetros:
Matriz* matricial -> ponteiro para a matriz a ser liberada.
*/
void liberar_matriz(Matriz *matricial) {

  if (matricial != NULL) {

    for (int i = 0; i < matricial->n; i++) {
      free(matricial->matriz[i]);
    }
    free(matricial->matriz);
    free(matricial);
  }
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
void gerar_grafo(Matriz *matricial, bool orientado, int probabilidade) {

  srand(time(NULL));

  if (!orientado) { // garante espelhamento
    for (int i = 0; i < matricial->n; i++) {
      for (int j = i; j < matricial->n; j++) {
        if (i != j) { // evitar ligacoes proprias
          matricial->matriz[i][j] = (rand() % 100 < probabilidade)
                                        ? (rand() % 9 + 1)
                                        : 0; // random de números entre 0 e 99

          // (resto da divisao por 100)
          matricial->matriz[j][i] = matricial->matriz[i][j];

        } else {
          matricial->matriz[i][j] =
              0; // falso para quando for a diagonal principal
        }
      }
    }
  } else {
    for (int i = 0; i < matricial->n; i++) {
      for (int j = 0; j < matricial->n; j++) {
        if (i != j) { // evitar ligacoes proprias
          matricial->matriz[i][j] = (rand() % 100 < probabilidade)
                                        ? 1 * (rand() % 9 + 1)
                                        : 0; // random de números entre 0 e 99
                                             // (resto da divisao por 100)
        } else {
          matricial->matriz[i][j] =
              0; // falso para quando for a diagonal principal
        }
      }
    }
  }
}

#pragma endregion grafo_aleatorio

#pragma region interação usuário

/*
Função que solicita ao usuário o preenchimento da probabilidade e da orientação
via prompt

Parâmetros:
float *prob -> ponteiro para se armazenar a probabilidade de cada aresta.
bool *orientado -> ponteiro para se armazenar a orientação do grafo.
int *qtd_vertices -> ponteiro para se armazenar a quantidade de vértices.
*/
void solicitar_ao_usuario(float *prob, bool *orientado, int *qtd_vertices) {

  do {

    printf(
        "\nDigite a probabilidade de existencia das arestas (entre 0 e 1): ");
    if (scanf(" %f", prob) != 1) { // Verificação do retorno de scanf
      printf("\nErro ao ler a probabilidade.\n");
      exit(1);
    }

    if ((*prob) < 0 || (*prob) > 1) {
      printf("\nDigite uma probabilidade valida.\n");
    }

  } while ((*prob < 0 || *prob > 1));

  *orientado = false; // nesse exercicio os grafos nao sao orientados

  do {

    printf("\nDigite a quantidade de vertices do grafo: ");
    if (scanf(" %d", qtd_vertices) != 1) { // Verificação do retorno de scanf
      printf("\nErro ao ler a quantidade de vertices.\n");
      exit(1);
    }

    if ((*qtd_vertices) < 1) {
      printf("\nDigite um número maior que 0.\n");
    }

  } while ((*qtd_vertices) < 1);
}

/*
Função para preenchimento manual das arestas de um grafo.

Parâmetros:
bool orientado -> booleano que informa se a matriz é orientada ou não.
Matriz* grafo -> ponteiro do grafo que receberá as posições.
*/
void preencher_manualmente(bool orientado, Matriz *grafo) {

  printf("Informe as distancias entre os vertices (use -1 para indicar "
         "ausencia de aresta):\n");
  for (int i = 0; i < grafo->n; i++) {

    for (int j = 0; j < grafo->n; j++) {

      if (i == j) {

        grafo->matriz[i][j] = 0;
      } else if (!orientado) {

        printf("Distancia entre o vertice %d e %d: ", i, j);
        scanf(" %d", &grafo->matriz[i][j]);
      } else if (i < j && orientado) {

        printf("Distancia entre o vertice %d e %d: ", i, j);
        scanf(" %d", &grafo->matriz[i][j]);
        grafo->matriz[j][i] = grafo->matriz[i][j];
      }

      if (grafo->matriz[i][j] == -1) {

        grafo->matriz[i][j] = 0;
      }
    }
  }
}

/*
Procedimento que cria a interface básica para o usuário interagir com o
programa.

Retorno:
Valor correspondente a escolha feita no menu.
*/
int menu() {
  int opcao;

  do {
    // printf(
    //     "\n\n+-------------------Menu---------------------+\n|1 - Inserir "
    //     "dados                           \n|2 - Carregar dados                "
    //     "          \n|3 - Criar caso de teste completo(RECOMENDADO)\n|4 - "
    //     "Inserir grafo manualmente               \n|5 - Aleatorizar grafo     "
    //     "                  \n|6 - Procurar clique maximal de um vertice   "
    //     "                        \n|7 - Salvar resultado em um arquivo                        \n|8 - Imprimir "
    //     "resultado na tela(Execute opcao 6 antes)              \n|0 - Sair                           "
    //     "         \n+--------------------------------------------+\n");

    printf(
        "\n\n+-------------------Menu---------------------+\n|1 - Inserir "
        "dados                           \n|2 - Aleatorizar grafo     "
        "                  \n|3 - Hipotese de Hierholzer   "
        "                        \n|4 - Visualizar matriz de adjacencias"
        "                        \n|0 - Sair                           "
        "         \n+--------------------------------------------+\n");
    scanf(" %d", &opcao);
  } while (opcao < 0 || opcao > 4);

  return opcao;
}

#pragma endregion interação usuário

#pragma region eulerizacao

// Função para verificar se o grafo é conexo usando busca em profundidade (DFS)
void dfs(Matriz* grafo, int v, int* visitado) {
    visitado[v] = 1;
    for (int i = 0; i < grafo->n; i++) {
        if (grafo->matriz[v][i] > 0 && !visitado[i]) {
            dfs(grafo, i, visitado);
        }
    }
}

int eh_conexo(Matriz* grafo) {
    int* visitado = (int*)calloc(grafo->n, sizeof(int));
    dfs(grafo, 0, visitado);
    for (int i = 0; i < grafo->n; i++) {
        if (!visitado[i]) {
            free(visitado);
            return 0;
        }
    }
    free(visitado);
    return 1;
}

int todos_graus_pares(Matriz* grafo) {
    for (int i = 0; i < grafo->n; i++) {
        int grau = 0;
        for (int j = 0; j < grafo->n; j++) {
            grau += grafo->matriz[i][j];
        }
        if (grau % 2 != 0) {
            return 0;
        }
    }
    return 1;
}

// Função para adicionar uma aresta ao grafo
void adicionar_aresta(Matriz* grafo, int u, int v) {
    grafo->matriz[u][v]++;
    grafo->matriz[v][u]++;
}

// Função para fazer a eulerização do grafo
void eulerizar(Matriz* grafo) {
    for (int i = 0; i < grafo->n; i++) {
        int grau = 0;
        for (int j = 0; j < grafo->n; j++) {
            grau += grafo->matriz[i][j];
        }
        if (grau % 2 != 0) {
            for (int j = i + 1; j < grafo->n; j++) {
                int grau_j = 0;
                for (int k = 0; k < grafo->n; k++) {
                    grau_j += grafo->matriz[j][k];
                }
                if (grau_j % 2 != 0) {
                    adicionar_aresta(grafo, i, j);
                    break;
                }
            }
        }
    }
}

#pragma endregion eulerizacao


#pragma regio lista dupla

typedef struct lista_dupla {
    int item;
    struct lista_dupla *prox;
    struct lista_dupla *ant;
} LISTA_DUPLA;

typedef LISTA_DUPLA *p_lista_dupla;

p_lista_dupla criar_lista_dupla() {
    return NULL;
}

void inserir_lista_dupla(p_lista_dupla *lista, int item) {
    p_lista_dupla aux = (p_lista_dupla)malloc(sizeof(LISTA_DUPLA));
    aux->item = item;
    aux->prox = *lista;
    aux->ant = NULL;
    if (*lista != NULL) {
        (*lista)->ant = aux;
    }
    *lista = aux;
}

void remover_lista_dupla(p_lista_dupla *lista, p_lista_dupla elem) {
    if (elem->ant != NULL) {
        elem->ant->prox = elem->prox;
    } else {
        *lista = elem->prox;
    }
    if (elem->prox != NULL) {
        elem->prox->ant = elem->ant;
    }
    free(elem);
}

void liberar_lista_dupla(p_lista_dupla *lista) {
    p_lista_dupla aux;
    while (*lista != NULL) {
        aux = *lista;
        *lista = aux->prox;
        free(aux);
    }
}

void imprimir_lista_dupla(p_lista_dupla lista) {
    while (lista != NULL) {
        printf("%d ", lista->item);
        lista = lista->prox;
    }
    printf("\n");
}

void remover_todas_repeticoes(p_lista_dupla *lista) {
    if (*lista == NULL) return;

    p_lista_dupla atual = *lista;

    // Percorre cada elemento da lista
    while (atual != NULL) {
        p_lista_dupla comparador = atual->prox;
        
        // Percorre todos os elementos após o "atual"
        while (comparador != NULL) {
            if (comparador->item == atual->item) {
                // Se encontrar um elemento duplicado, remove-o
                p_lista_dupla temp = comparador;
                
                // Ajusta os ponteiros para "pular" o elemento duplicado
                if (temp->ant != NULL) {
                    temp->ant->prox = temp->prox;
                }
                if (temp->prox != NULL) {
                    temp->prox->ant = temp->ant;
                }

                comparador = temp->prox; // Avança o comparador
                free(temp); // Libera o elemento duplicado
            } else {
                // Avança o comparador se não houver duplicação
                comparador = comparador->prox;
            }
        }
        
        // Avança para o próximo elemento da lista
        atual = atual->prox;
    }
}


#pragma endregion lista dupla

#pragma region hierholzer

// Função para encontrar o ciclo Euleriano usando o algoritmo de Hierholzer
void hierholzer(Matriz* grafo, int start) {
    p_lista_dupla C = criar_lista_dupla();
    p_lista_dupla H = criar_lista_dupla();
    int* vetor_graus = (int*)calloc(grafo->n, sizeof(int));

    // Inicializa os graus dos vértices
    for (int i = 0; i < grafo->n; i++) {
        for (int j = 0; j < grafo->n; j++) {
            vetor_graus[i] += grafo->matriz[i][j];
        }
    }

    // Passo 1: Escolha qualquer vértice v ∈ V
    int v = start;

    // Passo 2: Construa um ciclo C a partir do vértice v
    int inicio = v;
    inserir_lista_dupla(&C, v);
    while (true) {
        int u = -1;
        for (int i = 0; i < grafo->n; i++) {
            if (grafo->matriz[v][i] > 0) {
                u = i;
                break;
            }
        }
        if (u == -1) break;
        inserir_lista_dupla(&C, u);
        grafo->matriz[v][u]--;
        grafo->matriz[u][v]--;
        vetor_graus[v]--;
        vetor_graus[u]--;
        v = u;
    }
    // Verifica se o ciclo retorna ao vértice inicial
    if (v != inicio) {
        inserir_lista_dupla(&C, inicio);
    }

    // Passo 3: A1 ← A \ C
    // Passo 4: K ← (V, A1)
    // A1 é representado pela matriz de adjacência atualizada

    // Passo 5: enquanto A1 ≠ ∅ faça
    while (true) {
        bool arestas_restantes = false;
        for (int i = 0; i < grafo->n; i++) {
            if (vetor_graus[i] > 0) {
                arestas_restantes = true;
                break;
            }
        }
        if (!arestas_restantes) break;

        // Passo 6: Escolha um vértice v tal que d(v) > 0 e v ∈ C
        p_lista_dupla elem = C;
        while (elem != NULL) {
            if (vetor_graus[elem->item] > 0) {
                v = elem->item;
                break;
            }
            elem = elem->prox;
        }

        // Passo 7: Construa um ciclo H a partir do vértice v
        inserir_lista_dupla(&H, v);
        while (true) {
            int u = -1;
            for (int i = 0; i < grafo->n; i++) {
                if (grafo->matriz[v][i] > 0) {
                    u = i;
                    break;
                }
            }
            if (u == -1) break;
            inserir_lista_dupla(&H, u);
            grafo->matriz[v][u]--;
            grafo->matriz[u][v]--;
            vetor_graus[v]--;
            vetor_graus[u]--;
            v = u;
        }

        // Passo 8: A1 ← A1 \ H
        // A1 é representado pela matriz de adjacência atualizada

        // Passo 9: C ← H ∪ C
        p_lista_dupla temp = H;
        while (temp != NULL) {
            inserir_lista_dupla(&C, temp->item);
            temp = temp->prox;
        }

        // Passo 10: H ← ∅
        liberar_lista_dupla(&H);
    }

    // Remover duplas repetidas consecutivas
    remover_todas_repeticoes(&C);

    // Imprimir o ciclo Euleriano
    printf("Ciclo Euleriano: ");
    imprimir_lista_dupla(C);

    // Liberar memória
    liberar_lista_dupla(&C);
    liberar_lista_dupla(&H);
    free(vetor_graus);
}

#pragma endregion hierholzer

/*
Função para visualizar a matriz de adjacências do grafo.

Parâmetros:
Matriz* grafo -> ponteiro de grafo para acessar a matriz de adjacência.
*/
void visualizar_matriz(Matriz *grafo) {
    for (int i = 0; i < grafo->n; i++) {
        for (int j = 0; j < grafo->n; j++) {
            printf("%d ", grafo->matriz[i][j]);
        }
        printf("\n");
    }
}

#pragma region main

int main() { 

  Matriz *grafo = NULL;
  bool orientado = false;
  float prob = -1.0;
  int qtd_vertices = -1;
  contador_cliques = 0;
  int caso;

  while (true) {

    caso = menu();

    switch (caso) {
    case 1: // Inserir dados
      solicitar_ao_usuario(&prob, &orientado, &qtd_vertices);
      break;
     
    case 2: // Aleatorizar grafo
      if (qtd_vertices != -1) {

        if (grafo == NULL) {
          grafo = inicializar_matriz(qtd_vertices);
        }

        gerar_grafo(grafo, orientado, (int)(prob * 100));
      } else {
        printf("\nInsira os dados primeiro.\n");
      }
      break; 
    case 3: // Checar hipótese de hierholzer
      if (grafo == NULL) {

          printf("\nInsira os dados e aleatorize o grafo primeiro (opcoes 1 e 2).\n");
      } else {
          if (!eh_conexo(grafo)) {

              printf("O grafo nao eh conexo e, portanto, nao possui um ciclo Euleriano!\n");
          } else { 
              if (todos_graus_pares(grafo)) {

                  printf("O grafo eh conexo e todos os vertices tem grau par. Um ciclo Euleriano pode ser encontrado.\n");
                  hierholzer(grafo, 0);
              } else {
                
                  printf("O grafo eh conexo, mas nem todos os vertices tem grau par. Realizando a eulerizacao do grafo...\n");
                  eulerizar(grafo);
                  
                  printf("\nMatriz de adjacencias apos eulerizacao:\n");
                  visualizar_matriz(grafo);

                  printf("Eulerizacao concluida. Agora, todos os vertices tem grau par!\n"); 
                  hierholzer(grafo, 0); // Executa o algoritmo de Hierholzer após a eulerização
              }
          }
      }
      break;
    case 4: // Visualizar matriz de adjacências
      if (grafo == NULL) {

          printf("\nInsira os dados e aleatorize o grafo primeiro (opcoes 1 e 2).\n");
      } else {

          visualizar_matriz(grafo);
      }
      break;
    case 0:

      liberar_matriz(grafo);
      exit(0);
      break;
    }
  }
  return 0;
  
}

#pragma endregion main