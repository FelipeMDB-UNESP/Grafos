#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TESTES 5

int main() {

    int qtd_filhos;
    float prob;
    int profundidade;

    FILE* teste;
    char buffer[32];
    srand(time(NULL));
    for (int i=1; i<=TESTES; i++) {

        sprintf(buffer, "teste%d.txt", i);
        FILE* teste = fopen(buffer,"w");
        
        qtd_filhos = rand() %8 +1;
        prob = ((1.0)*(rand() % 100))/(100 * (1.0));
        profundidade = rand() %5;

        sprintf(buffer, "%d\n%.2f\n%d\n", qtd_filhos, prob, profundidade);

        fprintf(teste,"%s",buffer);
    }
    
}