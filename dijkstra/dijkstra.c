#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

minHeap* criarHeap(int capacidade) {
    minHeap* minH = malloc(sizeof(minHeap));
    minH->capacidade = capacidade;
    minH->tamanho = 0;
    minH->heapArray = malloc(capacidade * sizeof(elem));
    minH->pos = malloc(capacidade * sizeof(int));
    for (int i = 0; i < capacidade; i++) minH->pos[i] = -1;
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
    int menor = i, e = 2*i+1, d = 2*i+2;
    if (e < min->tamanho && min->heapArray[e].chave < min->heapArray[menor].chave) menor = e;
    if (d < min->tamanho && min->heapArray[d].chave < min->heapArray[menor].chave) menor = d;
    if (menor != i) { trocar(min, i, menor); heapifyD(min, menor); }
}

void heapifyU(minHeap* min, int i) {
    while (i > 0 && min->heapArray[(i-1)/2].chave > min->heapArray[i].chave) {
        trocar(min, i, (i-1)/2);
        i = (i-1)/2;
    }
}

void decreaseKey(minHeap* min, int vertice, int novaChave) {
    int i = min->pos[vertice];
    min->heapArray[i].chave = novaChave;
    heapifyU(min, i);
}

void inserirHeap(minHeap* min, int chave, int vertice) {
    if (min->pos[vertice] != -1) {
        if (chave < min->heapArray[min->pos[vertice]].chave)
            decreaseKey(min, vertice, chave);
        return;
    }
    int i = min->tamanho++;
    min->heapArray[i].chave = chave;
    min->heapArray[i].conteudo = vertice;
    min->pos[vertice] = i;
    heapifyU(min, i);
}

elem removeMin(minHeap* min) {
    elem minimo = min->heapArray[0];
    min->pos[minimo.conteudo] = -1;
    if (min->tamanho > 1) {
        min->heapArray[0] = min->heapArray[--min->tamanho];
        min->pos[min->heapArray[0].conteudo] = 0;
        heapifyD(min, 0);
    } else min->tamanho--;
    return minimo;
}

void relaxar(minHeap* min, int* dist, int* pred, int u, int v, int peso) {
    if (dist[v] > dist[u] + peso) {
        dist[v] = dist[u] + peso;
        pred[v] = u;
        if (min->pos[v] == -1) inserirHeap(min, dist[v], v);
        else decreaseKey(min, v, dist[v]);
    }
}

void Dijkstra(int** grafo, int nVertices, int s, int* visitas) {
    int* dist = malloc(nVertices * sizeof(int));
    int* pred = malloc(nVertices * sizeof(int));
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
        for (int v = 0; v < nVertices; v++)
            if (grafo[u][v] > 0 && !visitas[v])
                relaxar(heapMin, dist, pred, u, v, grafo[u][v]);
    }
    for (int i = 0; i < nVertices; i++)
        printf("%d:%d ", i+1, dist[i] == INT_MAX ? -1 : dist[i]);
    printf("\n");
    free(dist); free(pred);
    free(heapMin->heapArray); free(heapMin->pos); free(heapMin);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s -f <arquivo> -i <vertice>\n", argv[0]);
        return 1;
    }

    char *arquivo = NULL;
    int inicio = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
            arquivo = argv[++i];
        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
            inicio = atoi(argv[++i]);
    }

    if (!arquivo || inicio == -1) {
        printf("Parâmetros inválidos.\n");
        return 1;
    }

    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    int n, m;
    fscanf(fp, "%d %d", &n, &m);

    int** grafo = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        grafo[i] = calloc(n, sizeof(int));

    for (int i = 0; i < m; i++) {
        int u, v, w;
        fscanf(fp, "%d %d %d", &u, &v, &w);
        grafo[u-1][v-1] = w;
        grafo[v-1][u-1] = w;
    }
    fclose(fp);

    int* visitas = calloc(n, sizeof(int));
    Dijkstra(grafo, n, inicio - 1, visitas);

    for (int i = 0; i < n; i++) free(grafo[i]);
    free(grafo); free(visitas);
    return 0;
}
