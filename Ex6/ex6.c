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

/*
Função para imprimir o grafo num dado arquivo.

Parâmetros:
Matriz* grafo -> ponteiro de grafo para acessar a matriz de adjascência.
FILE* arq -> Ponteiro de arquivo.
*/
void imprimir_grafo_arquivo(Matriz *grafo, FILE *arq) {

  fputc('\n', arq);

  fputc('+', arq);
  for (int l = 0; l < grafo->n; l++) {
    fputc('-', arq);
    fputc('-', arq);
    fputc('-', arq);
    fputc('+', arq);
  }
  fputc('\n', arq);

  for (int i = 0; i < grafo->n; i++) {

    fputc('|', arq);

    for (int j = 0; j < grafo->n; j++) {

      fprintf(arq, " %d |", grafo->matriz[i][j]);
    }
    fputc('\n', arq);

    fputc('+', arq);
    for (int l = 0; l < grafo->n; l++) {
      fputc('-', arq);
      fputc('-', arq);
      fputc('-', arq);
      fputc('+', arq);
    }
    fputc('\n', arq);
  }
}

#pragma endregion grafo_aleatorio

#pragma region lista

/*Definição da struct usada na lista.

Campos:
int item -> Inteiro armazenado na lista.
struct lista *prox -> ponteiro para o próximo elemento da lista.*/
typedef struct lista {
  int item;
  struct lista *prox;
} LISTA;

/*Apelido para o ponteiro da lista.*/
typedef LISTA *p_lista;

/*
Função para inicializacao de uma lista.

Retorno:
Um ponteiro vazio.
*/
p_lista criar_lista() {

  p_lista list = NULL;
  return list;
}

/*
Função para inserir um elemento na lista.

Parâmetros:
p_lista* list -> Ponteiro do ponteiro da lista.
int item -> Número a ser inserido na lista.
*/
void inserir_lista(p_lista *list, int item) {

  p_lista aux = (p_lista)malloc(sizeof(LISTA));
  aux->item = item;
  aux->prox = *list;
  *list = aux; // realiza alteração do topo da lista sem retornar valor
}

/*
Função para remover um elemento na lista.

Parâmetros:
p_lista* list -> Ponteiro do ponteiro da lista.

Retorno:
Item retirado.
*/
int retirar_lista(p_lista *list) {

  int item;
  p_lista aux;

  item = (*list)->item;
  aux = (*list)->prox;

  free(*list);
  *list = aux; // realiza alteração do topo da lista sem retornar valor

  return item;
}

/*
Função para desfazer a lista.

Parâmetros:
p_lista* list -> Ponteiro do ponteiro da lista.
*/
void liberar_lista(p_lista *list) {

  p_lista aux;

  while (*list != NULL) {
    aux = *list;
    *list = aux->prox;
    free(aux);
  }
}

/*
Função para imprimir os itens da lista.

Parâmetros:
p_lista list -> Ponteiro da lista.
*/
void imprimir_lista(p_lista list) {

  printf("\nLista:");

  while (list != NULL) {
    printf(" %d", list->item);
    list = list->prox;
  }

  putchar('\n');
  putchar('\n');
}

/*
Função para imprimir os itens da lista em um arquivo txt.

Parâmetros:
p_lista list -> Ponteiro da lista.
FILE* arq -> Ponteiro do arquivo.
*/
void imprimir_lista_arquivo(p_lista list, FILE *arq) {

  fprintf(arq, "\nLista:");

  while (list != NULL) {
    fprintf(arq, " %d", list->item);
    list = list->prox;
  }

  fputc('\n', arq);
}
#pragma endregion lista

#pragma region interação usuário

/*
Função que solicita ao usuário o preenchimento da probabilidade e da orientação
via prompt

Parâmetros:
float *prob -> ponteiro para se armazenar a probabilidade de cada aresta.
bool *orientado -> ponteiro para se armazenar a orientação do grafo.
*/
void solicitar_ao_usuario(float *prob, bool *orientado, int *qtd_vertices) {

  do {

    printf(
        "\nDigite a probabilidade de existencia das arestas (entre 0 e 1): ");
    if (scanf(" %f", prob) != 1) { // Verificação do retorno de clae
      printf("\nErro ao ler a probabilidade.\n");
      exit(1);
    }

    if ((*prob) < 0 || (*prob) > 1) {
      printf("\nDigite uma probabilidade valida.\n");
    }

  } while ((*prob < 0 || *prob > 1));

  int aux;
  do {

    printf("\nDigite 1 se o grafo for orientado, e 0 caso contrário: ");
    if (scanf(" %d", &aux) != 1) { // Verificação do retorno de scanf
      printf("\nErro ao ler a orientação.\n");
      exit(1);
    }

    if ((aux) != 0 && (aux) != 1) {
      printf("\nDigite uma opção valida.\n");
    }

  } while ((aux) != 0 && (aux) != 1);

  *orientado = aux ? true : false;

  do {

    printf("\nDigite a quantidade de vértices do grafo: ");
    if (scanf(" %d", qtd_vertices) != 1) { // Verificação do retorno de scanf
      printf("\nErro ao ler a quantidade de vértices.\n");
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
Procedimento para desfazer os arquivos de teste gerados nos casos de teste.
*/
void deletar_arquivos_caso_teste() {

  char buffer[64];

  for (int i = contador_cliques; i > 0; i--) {

    sprintf(buffer, "Entrada%d.txt", i);
    remove(buffer);

    sprintf(buffer, "Saida%d.txt", i);
    remove(buffer);
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
    printf(
        "\n\n+-------------------Menu---------------------+\n|1 - Inserir "
        "dados                           \n|2 - Carregar dados                "
        "          \n|3 - Criar caso de teste completo(RECOMENDADO)\n|4 - "
        "Inserir grafo manualmente               \n|5 - Aleatorizar grafo     "
        "                  \n|6 - Procurar clique maximal de um vertice   "
        "                        \n|7 - Salvar resultado em um arquivo                        \n|8 - Imprimir "
        "resultado na tela(Execute opcao 6 antes)              \n|0 - Sair                           "
        "         \n+--------------------------------------------+\n");
    scanf(" %d", &opcao);
  } while (opcao < 0 || opcao > 8);

  return opcao;
}

#pragma endregion interação usuário

#pragma region euler_hierholzer

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

int tem_graus_pares(Matriz* grafo) {
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

// Função para encontrar o ciclo Euleriano usando o algoritmo de Hierholzer
void hierholzer(Matriz* grafo, int start) {
    int* stack = (int*)malloc(grafo->n * sizeof(int));
    int* cycle = (int*)malloc(grafo->n * sizeof(int));
    int top = 0, cycle_index = 0;

    stack[top++] = start;

    while (top > 0) {
        int v = stack[top - 1];
        int i;
        for (i = 0; i < grafo->n; i++) {
            if (grafo->matriz[v][i] > 0) {
                stack[top++] = i;
                grafo->matriz[v][i]--;
                grafo->matriz[i][v]--;
                break;
            }
        }
        if (i == grafo->n) {
            cycle[cycle_index++] = v;
            top--;
        }
    }

    printf("Ciclo Euleriano: ");
    for (int i = cycle_index - 1; i >= 0; i--) {
        printf("%d ", cycle[i] + 1);
    }
    printf("\n");

    free(stack);
    free(cycle);
}

#pragma endregion euler_hierholzer

int main() {

  Matriz *grafo = NULL;
  p_lista list = NULL;
  bool orientado = false;
  float prob = -1.0;
  int vertice = -1;
  int qtd_vertices = -1;
  contador_cliques = 0;
  int caso;

  while (true) {

    caso = menu();

    switch (caso) {
    case 1:
      solicitar_ao_usuario(&prob, &orientado, &qtd_vertices);
      break;
    case 2:
      solicitar_ao_arquivo(&prob, &orientado, &qtd_vertices);
      break;
    case 3:
      caso_teste(&orientado, &prob, &qtd_vertices);
      break;
    case 4:
      if (qtd_vertices != -1) {
        if (grafo == NULL)
          grafo = inicializar_matriz(qtd_vertices);

        preencher_manualmente(orientado, grafo);
      } else {
        printf("\nInsira os dados primeiro.\n");
      }
      break;
    case 5:
      if (qtd_vertices != -1) {

        if (grafo == NULL) {
          grafo = inicializar_matriz(qtd_vertices);
        }

        gerar_grafo(grafo, orientado, (int)(prob * 100));
      } else {
        printf("\nInsira os dados primeiro.\n");
      }
      break;
    case 6:
      if (grafo != NULL) {

        do {
          printf("\nDigite o número do vértice (entre 1 e %d): ", grafo->n);
          scanf(" %d", &vertice);

        } while (vertice < 1 || vertice > grafo->n);

        list = clique_maximal(grafo, vertice - 1);
        contador_cliques++;
      } else {
        printf("\nCrie o grafo primeiro.\n");
      }
      break;
    case 7:
      imprimir_resultado_arquivo(list);
      break;
    case 8:
      printf("\nOs vértices do último clique gerado são: ");
      imprimir_lista(list);
      break;
    case 0:
      if (contador_cliques > 0)
        deletar_arquivos_caso_teste();
      liberar_matriz(grafo);
      liberar_lista(&list);
      exit(0);
      break;
    }
  }
  return 0;
}