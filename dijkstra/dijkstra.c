#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int chave;
    int conteudo;
} elem;

typedef struct {
    int capacidade;
    int tamanho;
    elem* heapArray;
    int* pos;
} minHeap;

no* criarNo(int vertice, int peso) {
    no* novoNo = malloc(sizeof(no));
    novoNo->vertice = vertice;
    novoNo->peso = peso;
    novoNo->prox = NULL;
    return novoNo;
}

minHeap* criarHeap(int capacidade) {
    minHeap* minH = malloc(sizeof(minHeap));
    minH->capacidade = capacidade;
    minH->tamanho = 0;
    minH->heapArray = malloc(capacidade * sizeof(elem));
    minH->pos = malloc(capacidade * sizeof(int));
    
    for (int i = 0; i < capacidade; i++) {
        minH->pos[i] = -1;
    }
    return minH;
}

void trocar(minHeap* minH, int a, int b) {
    elem aux = minH->heapArray[a];
    minH->heapArray[a] = minH->heapArray[b];
    minH->heapArray[b] = aux;
    minH->pos[minH->heapArray[a].conteudo] = a;
    minH->pos[minH->heapArray[b].conteudo] = b;
}

void heapifyD(minHeap* min, int i) {
    int menor = i;
    int filhoE = 2 * i + 1;
    int filhoD = 2 * i + 2;

    if (filhoE < min->tamanho && min->heapArray[filhoE].chave < min->heapArray[menor].chave) {
        menor = filhoE;
    }

    if (filhoD < min->tamanho && min->heapArray[filhoD].chave < min->heapArray[menor].chave) {
        menor = filhoD;
    }

    if (menor != i) {
        trocar(min, i, menor);
        heapifyD(min, menor);
    }
}

void heapifyU(minHeap* min, int i) {
    while (i > 0 && min->heapArray[(i-1)/2].chave > min->heapArray[i].chave) {
        trocar(min, i, (i-1)/2);
        i = (i-1)/2;
    }
}

void inserirHeap(minHeap* min, int chave, int vertice) {
    if (min->pos[vertice] != -1) {
        if (chave < min->heapArray[min->pos[vertice]].chave) {
            decreaseKey(min, vertice, chave);
        }
        return;
    }

    int i = min->tamanho++;
    min->heapArray[i].chave = chave;
    min->heapArray[i].conteudo = vertice;
    min->pos[vertice] = i;
    heapifyU(min, i);
}

void decreaseKey(minHeap* min, int vertice, int novaChave) {
    int i = min->pos[vertice];
    min->heapArray[i].chave = novaChave;
    heapifyU(min, i);
}

elem removeMin(minHeap* min) {
    elem minimo = min->heapArray[0];
    min->pos[minimo.conteudo] = -1;
    min->heapArray[0] = min->heapArray[--min->tamanho];
    min->pos[min->heapArray[0].conteudo] = 0;
    heapifyD(min, 0);
    return minimo;
}

void relaxar(minHeap* min, int* dist, int* pred, int u, int v, int peso) {
    if (dist[v] > dist[u] + peso) {
        dist[v] = dist[u] + peso;
        pred[v] = u;
        
        if (min->pos[v] == -1) {
            inserirHeap(min, dist[v], v);
        } else {
            decreaseKey(min, v, dist[v]);
        }
    }
}

void Dijkstra(int** grafo, int nVertices, int s, int* visitas) {
    int dist[nVertices];
    int pred[nVertices];

    for (int i = 0; i < nVertices; i++) {
        dist[i] = INT_MAX;
        pred[i] = -1;
        visitas[i] = 0;
    }
    dist[s] = 0;

    minHeap* heapMin = criarHeap(nVertices);
    inserirHeap(heapMin, 0, s);

    while (heapMin->tamanho > 0) {
        elem menor = removeMin(heapMin);
        int u = menor.conteudo;

        if (visitas[u]) continue;
        visitas[u] = 1;

        for (int v = 0; v < nVertices; v++) {
            if (grafo[u][v] > 0 && !visitas[v]) {
                relaxar(heapMin, dist, pred, u, v, grafo[u][v]);
            }
        }
    }

    for (int i = 0; i < nVertices; i++) {
        if (dist[i] == INT_MAX)
            printf("%d:-1 ", i+1);
        else
            printf("%d:%d ", i+1, dist[i]);
    }

    free(heapMin->heapArray);
    free(heapMin->pos);
    free(heapMin);
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    
    int** grafo = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        grafo[i] = calloc(n, sizeof(int));
    }

    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        grafo[u-1][v-1] = w;
        grafo[v-1][u-1] = w;
    }

    int inicio;
    scanf("%d", &inicio);
    int* visitas = calloc(n, sizeof(int));
    Dijkstra(grafo, n, inicio-1, visitas);

    for (int i = 0; i < n; i++){
        free(grafo[i]);}
    free(grafo);
    free(visitas);

    return 0;
}