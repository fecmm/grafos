#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/kosaraju.h"

#define MAX_VERTICES MAX


typedef struct componente {
    int* nos;
    int tamanho;
    int menor_no;
} Componente;

int pilhaVertices[MAX_VERTICES];
int indiceTopo = -1;

no* novoNo(int id) {
    struct no* novo = malloc(sizeof(no));
    novo->vertice = id;
    novo->prox = NULL;
    return novo;
}

grafo* inicializarGrafo(int totalNos) {
    struct grafo* g = malloc(sizeof(grafo));
    g->nVertices = totalNos;
    g->adLists = malloc(totalNos * sizeof(no*));
    g->visitas = malloc(totalNos * sizeof(int));

    for (int i = 0; i < totalNos; i++) {
        g->adLists[i] = NULL;
        g->visitas[i] = 0;
    }

    return g;
}

void adicionarAresta(grafo* g, int origem, int destino) {
    struct no* novo = novoNo(destino);
    novo->prox = g->adLists[origem];
    g->adLists[origem] = novo;
}

void push(int vertice) {
    pilhaVertices[++indiceTopo] = vertice;
}

int pop() {
    return pilhaVertices[indiceTopo--];
}

int compararVertices(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

grafo* inverterGrafo(grafo* g) {
    grafo* invertido = inicializarGrafo(g->nVertices);

    for(int v = 0; v < g->nVertices; v++) {
        no* atual = g->adLists[v];
        while(atual != NULL) {
            adicionarAresta(invertido, atual->vertice, v);
            atual = atual->prox;
        }
    }

    return invertido;
}

void buscaProfundidade(grafo* g, int vertice) {
    g->visitas[vertice] = 1;
    no* adjacente = g->adLists[vertice];

    while(adjacente != NULL) {
        if(g->visitas[adjacente->vertice] == 0) {
            buscaProfundidade(g, adjacente->vertice);
        }
        adjacente = adjacente->prox;
    }

    push(vertice);
}

void buscaProfundidadeInvertida(grafo* g, int vertice, int* componente, int* tamComponente) {
    g->visitas[vertice] = 1;
    componente[(*tamComponente)++] = vertice + 1;

    no* adjacente = g->adLists[vertice];
    while (adjacente != NULL) {
        if (g->visitas[adjacente->vertice] == 0) {
            buscaProfundidadeInvertida(g, adjacente->vertice, componente, tamComponente);
        }
        adjacente = adjacente->prox;
    }
}

int compararComponentes(const void* a, const void* b) {
    Componente* comp1 = *(Componente**)a;
    Componente* comp2 = *(Componente**)b;

    if (comp1->tamanho == 1 && comp1->menor_no == 1) return -1;
    if (comp2->tamanho == 1 && comp2->menor_no == 1) return 1;

    if (comp1->tamanho != comp2->tamanho) {
        return comp2->tamanho - comp1->tamanho;
    }

    return comp1->menor_no - comp2->menor_no;
}

void kosaraju(grafo* g, FILE* output) {
    Componente* componentes[MAX_VERTICES];
    int numComponentes = 0;

    for(int v = 0; v < g->nVertices; v++) {
        if(g->visitas[v] == 0) {
            buscaProfundidade(g, v);
        }
    }

    grafo* grafoInvertido = inverterGrafo(g);

    for(int v = 0; v < g->nVertices; v++) {
        g->visitas[v] = 0;
    }

    while(indiceTopo != -1) {
        int v = pop();

        if(grafoInvertido->visitas[v] == 0) {
            int componente[MAX_VERTICES];
            int tamComponente = 0;

            buscaProfundidadeInvertida(grafoInvertido, v, componente, &tamComponente);
            qsort(componente, tamComponente, sizeof(int), compararVertices);

            Componente* novaComp = (Componente*) malloc(sizeof(Componente));
            novaComp->nos = (int*) malloc(tamComponente * sizeof(int));
            memcpy(novaComp->nos, componente, tamComponente * sizeof(int));
            novaComp->tamanho = tamComponente;
            novaComp->menor_no = componente[0];

            componentes[numComponentes++] = novaComp;
        }
    }

    qsort(componentes, numComponentes, sizeof(Componente*), compararComponentes);

    for (int i = 0; i < numComponentes; i++) {
        if (i > 0) fprintf(output, "\n");

        for (int j = 0; j < componentes[i]->tamanho; j++) {
            fprintf(output, "%d ", componentes[i]->nos[j]);
        }
        
        free(componentes[i]->nos);
        free(componentes[i]);
    }
}
