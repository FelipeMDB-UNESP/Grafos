#include "vertice.h"
#include <iostream>
using namespace std;

int main(){

    ifstream f("exemplo.dat");
    grafo G(7,f);
    G.imprime();
    
    vector<int> vistd = bl_ordem(G);
    cout << endl;
    imprime(vistd);
    return 0;
}