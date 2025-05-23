#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/kosaraju.h"

void mostrarAjuda() {
    printf("Uso: ./kosaraju [opcoes]\n");
    printf("  -h           : mostra o help\n");
    printf("  -o <arquivo> : redireciona a saida para o arquivo\n");
    printf("  -f <arquivo> : arquivo com o grafo de entrada\n");
}

grafo* lerGrafo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    int totalNos, totalArestas;
    fscanf(arquivo, "%d %d", &totalNos, &totalArestas);

    grafo* g = inicializarGrafo(totalNos);

    for (int i = 0; i < totalArestas; i++) {
        int origem, destino;
        fscanf(arquivo, "%d %d", &origem, &destino);
        adicionarAresta(g, origem - 1, destino - 1);
    }

    fclose(arquivo);
    return g;
}

int main(int argc, char *argv[]) {
    char* arquivoEntrada = NULL;
    char* arquivoSaida = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            mostrarAjuda();
            return 0;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            arquivoEntrada = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            arquivoSaida = argv[++i];
        }
    }

    if (!arquivoEntrada) {
        printf("Erro: Arquivo de entrada nao especificado. Use -f <arquivo>.\n");
        return 1;
    }

    FILE* saida = stdout;
    if (arquivoSaida) {
        saida = fopen(arquivoSaida, "w");
        if (!saida) {
            perror("Erro ao abrir arquivo de saida");
            return 1;
        }
    }

    grafo* grafo = lerGrafo(arquivoEntrada);
    kosaraju(grafo, saida);

    if (arquivoSaida) fclose(saida);

    return 0;
}