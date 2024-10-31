#include <vector>
#include <fstream>

using namespace std;
#ifndef VERTICE_H
#define VERTICE_H

class vertice{

public:
    int id; //índice do nó no grafo
    vector<vertice*> viz; //lista de vizinhos

    vertice();
    vertice(int id);

    vertice* operator[](int k){
        return this->viz[k];
    }

    bool ehVizinho(vertice* v);

};

class grafo{
public:
    int N;
    vertice* V;

    grafo();
    grafo(int N, ifstream& f);
    ~grafo();

    vertice& operator[](int k){
        return this->V[k];
    }

    void imprime();
};

bool estaEm(vector<int>* v, int k);

vector<int> bp_ordem(grafo& G);
vector<int> bl_ordem(grafo& G);

void imprime(vector<int>& visitd);

#endif