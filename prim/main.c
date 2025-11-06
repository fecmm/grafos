#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/prim.h"


void imprimirAjuda() {
    printf("Uso: ./prim [opções]\n");
    printf("  -h           : exibe ajuda\n");
    printf("  -f <arquivo> : arquivo do grafo\n");
    printf("  -i <vértice> : vértice inicial\n");
    printf("  -s           : exibe a árvore geradora mínima\n");
    printf("  -o <arquivo> : arquivo para saída\n");
}

grafo* carregarGrafoDeArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    int nVertices, nArestas, origem, destino, peso;
    fscanf(arquivo, "%d %d", &nVertices, &nArestas);

    grafo* grafoLido = criarGrafo(nVertices);

    for (int i = 0; i < nArestas; i++) {
        fscanf(arquivo, "%d %d %d", &origem, &destino, &peso);
        addAresta(grafoLido, origem - 1, destino - 1, peso);
    }

    fclose(arquivo);
    return grafoLido;
}

void imprimirArvoreGeradora(grafo* agm) {
    for (int i = 0; i < agm->nVertices; i++) {
        no* adjacente = agm->adLists[i];
        while (adjacente) {
            if (i < adjacente->vertice) {
                printf("(%d,%d) ", i + 1, adjacente->vertice + 1);
            }
            adjacente = adjacente->prox;
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    char *nomeEntrada = NULL, *nomeSaida = NULL;
    int verticeInicial = 0;
    int mostrarArvore = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            imprimirAjuda();
            return 0;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            nomeEntrada = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            nomeSaida = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            verticeInicial = atoi(argv[++i]) - 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            mostrarArvore = 1;
        }
    }

    if (!nomeEntrada) {
        fprintf(stderr, "Parâmetros obrigatórios ausentes. Use -h para ajuda.\n");
        return 1;
    }

    if (verticeInicial < 0) {
        verticeInicial = 0;
    }

    FILE* saida = stdout;
    if (nomeSaida) {
        saida = fopen(nomeSaida, "w");
        if (!saida) {
            perror("Erro ao abrir arquivo de saída");
            return 1;
        }
    }

    grafo* grafoOriginal = carregarGrafoDeArquivo(nomeEntrada);
    grafo* agm = prim(grafoOriginal, verticeInicial);

    if (mostrarArvore) {
        if (nomeSaida) fclose(saida);
        saida = nomeSaida ? fopen(nomeSaida, "a") : stdout;
        for (int v = 0; v < agm->nVertices; v++) {
            no* temp = agm->adLists[v];
            while (temp) {
                if (v < temp->vertice) {
                    fprintf(saida, "(%d,%d) ", v + 1, temp->vertice + 1);
                }
                temp = temp->prox;
            }
        }
        fprintf(saida, "\n");
    } else {
        printf("%d\n", agm->custoTotal);
    }

    liberarGrafo(grafoOriginal);
    liberarGrafo(agm);

    if (nomeSaida && saida != stdout) fclose(saida);

    return 0;
}
