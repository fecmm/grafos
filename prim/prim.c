#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../headers/prim.h"

no* criarNo(int vertice, int peso) {
    no* novoNo = malloc(sizeof(no));
    novoNo->vertice = vertice;
    novoNo->peso = peso;
    novoNo->prox = NULL;
    return novoNo;
}


grafo* criarGrafo(int numeroVertices) {
    grafo* novoGrafo = malloc(sizeof(grafo));
    novoGrafo->nVertices = numeroVertices;
    novoGrafo->nArestas = 0;
    novoGrafo->adLists = malloc(numeroVertices * sizeof(no*));
    novoGrafo->visitas = malloc(numeroVertices * sizeof(int));
    novoGrafo->custoTotal = 0;

    for (int i = 0; i < numeroVertices; i++) {
        novoGrafo->adLists[i] = NULL;
        novoGrafo->visitas[i] = 0;
    }

    return novoGrafo;
}

minHeap* criarMinHeap(int capacidade) {
    minHeap* heap = malloc(sizeof(minHeap));
    heap->capacidade = capacidade;
    heap->tamanho = 0;
    heap->vertices = malloc(capacidade * sizeof(int));
    heap->pos = malloc(capacidade * sizeof(int));

    for (int i = 0; i < capacidade; i++) {
        heap->vertices[i] = -1;
        heap->pos[i] = -1;
    }

    return heap;
}

void addAresta(grafo* grafo, int origem, int destino, int peso) {
    no* novoNoOrigem = criarNo(origem, peso);
    novoNoOrigem->prox = grafo->adLists[destino];
    grafo->adLists[destino] = novoNoOrigem;

    no* novoNoDestino = criarNo(destino, peso);
    novoNoDestino->prox = grafo->adLists[origem];
    grafo->adLists[origem] = novoNoDestino;
}

void trocar(minHeap* heap, int posA, int posB) {
    int temp = heap->vertices[posA];
    heap->vertices[posA] = heap->vertices[posB];
    heap->vertices[posB] = temp;

    heap->pos[heap->vertices[posA]] = posA;
    heap->pos[heap->vertices[posB]] = posB;
}

void inserirHeap(minHeap* heap, int vertice, int* chave) {
    int i = heap->tamanho;
    heap->vertices[i] = vertice;
    heap->pos[vertice] = i;
    heap->tamanho++;

    while (i != 0 && chave[heap->vertices[(i - 1) / 2]] > chave[heap->vertices[i]]) {
        trocar(heap, (i - 1) / 2, i);
        i = (i - 1) / 2;
    }
}

void diminuirChave(minHeap* heap, int vertice, int* chave) {
    int i = heap->pos[vertice];
    while (i != 0 && chave[heap->vertices[(i - 1) / 2]] > chave[heap->vertices[i]]) {
        trocar(heap, (i - 1) / 2, i);
        i = (i - 1) / 2;
    }
}

void heapify(minHeap* heap, int i, int* chave) {
    int menor = i;
    int filhoEsquerdo = 2 * i + 1;
    int filhoDireito = 2 * i + 2;

    if (filhoEsquerdo < heap->tamanho && chave[heap->vertices[filhoEsquerdo]] < chave[heap->vertices[menor]]) {
        menor = filhoEsquerdo;
    }
    if (filhoDireito < heap->tamanho && chave[heap->vertices[filhoDireito]] < chave[heap->vertices[menor]]) {
        menor = filhoDireito;
    }
    if (menor != i) {
        trocar(heap, i, menor);
        heapify(heap, menor, chave);
    }
}

int removerMinimo(minHeap* heap, int* chave) {
    if (heap->tamanho == 0) {
        exit(1);
    }

    int verticeRemovido = heap->vertices[0];
    int verticeUltimo = heap->vertices[heap->tamanho - 1];

    heap->vertices[0] = verticeUltimo;
    heap->pos[verticeUltimo] = 0;
    heap->pos[verticeRemovido] = -1;

    heap->tamanho--;

    heapify(heap, 0, chave);

    return verticeRemovido;
}

grafo* prim(grafo* grafoOriginal, int verticeInicial) {
    int* chave = malloc(grafoOriginal->nVertices * sizeof(int));
    int* anterior = malloc(grafoOriginal->nVertices * sizeof(int));
    minHeap* heap = criarMinHeap(grafoOriginal->nVertices);

    for (int i = 0; i < grafoOriginal->nVertices; i++) {
        chave[i] = INT_MAX;
        anterior[i] = -1;
        heap->pos[i] = -1;
    }

    chave[verticeInicial] = 0;
    inserirHeap(heap, verticeInicial, chave);

    grafo* agm = criarGrafo(grafoOriginal->nVertices);

    while (heap->tamanho != 0) {
        int v = removerMinimo(heap, chave);

        if (grafoOriginal->visitas[v] == 1)
            continue;

        grafoOriginal->visitas[v] = 1;

        if (anterior[v] != -1) {
            addAresta(agm, anterior[v], v, chave[v]);
            agm->custoTotal += chave[v];
            agm->nArestas++;
        }

        no* adjacente = grafoOriginal->adLists[v];

        while (adjacente != NULL) {
            int u = adjacente->vertice;

            if (!grafoOriginal->visitas[u] && adjacente->peso < chave[u]) {
                chave[u] = adjacente->peso;
                anterior[u] = v;

                if (heap->pos[u] == -1) {
                    inserirHeap(heap, u, chave);
                } else {
                    diminuirChave(heap, u, chave);
                }
            }

            adjacente = adjacente->prox;
        }
    }

    free(anterior);
    free(chave);
    free(heap->vertices);
    free(heap->pos);
    free(heap);

    return agm;
}

void liberarGrafo(grafo* grafo) {
    if (!grafo)
        return;

    for (int i = 0; i < grafo->nVertices; i++) {
        no* atual = grafo->adLists[i];
        while (atual != NULL) {
            no* proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }

    free(grafo->adLists);
    free(grafo->visitas);
    free(grafo);
}
