
#include "vertice.h"
#include <cstddef>
#include <fstream>
#include <iostream>

/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------
    Funções de vertices
--------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

vertice::vertice(){}
vertice::vertice(int id){
    this->id = id;
}

bool vertice::ehVizinho(vertice *v){
    for (int i=0; i<this->viz.size(); i++){
        if (v==this->viz.at(i)){
            return true;
        }
    }
    return false;
}

/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------
    Funções de grafos
--------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

grafo::grafo(){
    this->N = 0;
    this->V = nullptr;
}

grafo::grafo(int N, ifstream& f){
    this->N = N;
    this->V = new vertice[N];
    while(!f.eof()){
        int i, j;
        f >> i; f >> j;
        this->V[i].id = i;
        this->V[i].viz.push_back(&this->V[j]);
        this->V[j].id = j;
        this->V[j].viz.push_back(&this->V[i]);
    }
}

grafo::~grafo(){
    delete[] this->V;
}

void grafo::imprime(){
    for (int i=0; i<this->N; i++){
        cout << i << " ";
        for (int j=0; j<this->V[i].viz.size(); j++){
            cout << this->V[i].viz[j]->id << " ";
        }
        cout << endl;
    }
}

/*------------------------------------------------------------------------------
--------------------------------------------------------------------------------
    Outras
--------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

bool estaEm(vector<int>& v, int k){
    for (int i=0; i<v.size(); i++){
        if (v[i]==k)
            return true;
    }
    return false;
}

void bp_visit(grafo& G, int raiz, int pai, vector<int>& visitd){
    for (int j=0; j<G[raiz].viz.size(); j++){
        if (G[raiz].viz[j]->id!=pai){
            bp_visit(G,G[raiz].viz[j]->id,raiz,visitd);
        }
    }
    visitd.insert(visitd.begin(),raiz);
}
vector<int> bp_ordem(grafo& G){
    vector<int> visitd;
    for (int i=0; i<G.N; i++){
        if (!estaEm(visitd,i)){
            bp_visit(G,i,-1,visitd);
        }
    }
    return visitd;
}

void imprime(vector<int>& visitd){
    for (int i=0; i<visitd.size(); i++){
        cout << visitd[i] << endl;
    }
}