#ifndef KOSARAJU_H
#define KOSARAJU_H

#define MAX_VERTICES 1000

typedef struct no {
    int vertice;
    struct no* prox;
} no;

typedef struct grafo {
    int totalNos;
    struct no** listaAdjacencia;
    int* visitados;
} grafo;

typedef struct {
    int* nos; 
    int tamanho;   
    int menorNo; 
} Componente;

extern int pilhaVertices[MAX_VERTICES];
extern int indiceTopo;

no* novoNo(int id);
grafo* inicializarGrafo(int totalNos);
void push(int vertice);
int pop();
void adicionarAresta(grafo* g, int origem, int destino);
int compararVertices(const void* a, const void* b);
grafo* inverterGrafo(grafo* g);
void buscaProfundidade(grafo* g, int vertice);
void buscaProfundidadeInvertida(grafo* g, int vertice, int* componente, int* tamComponente);
int compararComponentes(const void* a, const void* b);
void kosaraju(grafo* g, FILE* saida);

#endif