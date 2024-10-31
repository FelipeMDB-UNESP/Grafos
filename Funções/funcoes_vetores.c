/*
Por:
André Luis Dias Nogueira
Felipe Melchior de Britto
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
