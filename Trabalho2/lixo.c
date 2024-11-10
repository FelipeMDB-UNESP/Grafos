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

#pragma region clique

/*
Função para checar se todos os elementos da lista possuem o vértice inserido
como vizinho.

Parâmetros:
Matriz* grafo -> ponteiro da matriz de adjascências do grafo.
p_lista* list -> Ponteiro do ponteiro da lista.
int vertice -> Vértice do grafo.

Retorno:
Booleano contendo verdadeiro ou falso para a busca.
*/
bool conectado(Matriz *grafo, p_lista list, int vertice) {

  while (list != NULL) {

    if (!(grafo->matriz[vertice][list->item] ||
          grafo->matriz[list->item][vertice]))
      return false;

    list = list->prox;
  }
  return true;
}

/*
Função para encontrar o subgrafo completo maximal que contém o vértice dado.

Parâmetros:
Matriz* grafo -> ponteiro da matriz de adjascências do grafo.
int vertice -> Vértice do grafo.

Retorno:
Ponteiro para a lista contendo os vértices do clique.
*/
p_lista clique_maximal(Matriz *grafo, int vertice) {

  p_lista list = criar_lista();
  inserir_lista(&list, vertice);

  for (int item = 0; item < grafo->n; item++) {

    if (item != vertice && conectado(grafo, list, item))
      inserir_lista(&list, item);
  }
  return list;
}

#pragma endregion clique

#pragma region interação usuário

/*
Função para imprimir o resultado do clique maximal em um arquivo.

Parâmetros:
p_lista list -> Ponteiro da lista.
*/
void imprimir_resultado_arquivo(p_lista list) {

  char buffer[32];

  printf("\nDigite o nome do arquivo texto a exportar os dados (adicionar .txt "
         "no fim):\n");
  scanf(" %s", buffer);

  FILE *teste = fopen(buffer, "a+");

  fprintf(teste,
          "\nLista dos vértices do clique maximal %d:", contador_cliques);
  imprimir_lista_arquivo(list, teste);

  fclose(teste);
}

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
Função que solicita ao usuário o nome do arquivo a ser carregado

Parâmetros:
float *prob -> ponteiro para se armazenar a probabilidade de cada aresta.
bool *orientado -> ponteiro para se armazenar a orientação do grafo.
*/
void solicitar_ao_arquivo(float *prob, bool *orientado, int *qtd_vertices) {

  char buffer[16];

  printf("\nDigite o nome do arquivo texto a extrair os dados:\n");
  scanf(" %s", buffer);

  FILE *teste = fopen(buffer, "r+");
  if (teste == NULL) {
    printf("Falha ao abrir o arquivo.\n");
    exit(1);
  }

  fgets(buffer, 16, teste);
  *prob = atof(buffer);

  fgets(buffer, 16, teste);
  *orientado = atoi(buffer) ? true : false;

  fgets(buffer, 16, teste);
  *qtd_vertices = atoi(buffer);

  fclose(teste);
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
Função para fazer um teste completo aleatório. Cria arquivos de entrada e saída
usados.

Parâmetros:
bool *orientado -> booleano que informa se a matriz é orientada ou não.
float *prob -> probabilidade de cada aresta.
int *qtd_vertices -> quantidade de vertices do grafo.
*/
void caso_teste(bool *orientado, float *prob, int *qtd_vertices) {

  srand(time(NULL));

  *orientado = rand() % 2 ? true : false;
  *prob = ((1.0 * (rand() % 101)) / (100.0));
  *qtd_vertices = rand() % 20 + 1;

  char buffer[64];
  contador_cliques++;

  sprintf(buffer, "Entrada%d.txt", contador_cliques);
  FILE *entrada = fopen(buffer, "w");

  sprintf(buffer, "%.2f\n%d\n%d\n", *prob, (*orientado ? 1 : 0), *qtd_vertices);
  fprintf(entrada, "%s", buffer);
  fclose(entrada);

  Matriz *grafo = inicializar_matriz(*qtd_vertices);
  grafo->n = *qtd_vertices;
  gerar_grafo(grafo, *orientado, (int)((*prob) * 100));

  int vertice = (rand() % (*qtd_vertices));
  p_lista list = clique_maximal(grafo, vertice);

  sprintf(buffer, "Saida%d.txt", contador_cliques);
  FILE *saida = fopen(buffer, "w");

  fprintf(saida, "Lista dos vértices do clique maximal %d:", contador_cliques);
  imprimir_lista_arquivo(list, saida);

  imprimir_grafo_arquivo(grafo, saida);

  fclose(saida);
  liberar_matriz(grafo);
  liberar_lista(&list);
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