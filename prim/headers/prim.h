#ifndef PRIM_H
#define PRIM_H

typedef struct no {
    int vertice;
    int peso;
    struct no* prox;
} no;


typedef struct grafo {
    int nVertices;
    int nArestas;
    no** adLists;
    int* visitas;
    int custoTotal;
} grafo;

typedef struct minHeap {
    int capacidade;
    int tamanho;
    int* vertices;
    int* pos;
} minHeap;

grafo* criarGrafo(int numeroVertices);
no* criarNo(int vertice, int peso);
void addAresta(grafo* grafo, int origem, int destino, int peso);
grafo* prim(grafo* grafoOriginal, int verticeInicial);
void liberarGrafo(grafo* grafo);

#endif
