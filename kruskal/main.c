#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/kruskal.h"

#define MAX_VERTICES 1000

void show_help() {
    printf("Usage: ./kruskal [options]\n");
    printf("  -h           : show this help message\n");
    printf("  -f <file>    : input graph file\n");
    printf("  -s           : show minimum spanning tree (MST)\n");
    printf("  -o <file>    : redirect output to file\n");
}


void load_edges(const char* filename, int* vertex_count, int* edge_file_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d %d", vertex_count, edge_file_count) != 2) {
        fprintf(stderr, "Error reading V and E from file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    extern int edge_count;
    edge_count = 0;

    for (int i = 0; i < *edge_file_count; i++) {
        int source, dest, weight;
        if (fscanf(file, "%d %d %d", &source, &dest, &weight) != 3) {
            fprintf(stderr, "Error reading edge %d from file %s\n", i + 1, filename);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        
        add_edge(source - 1, dest - 1, weight);
    }

    fclose(file);
}

void print_mst(Edge* mst, int mst_edge_count, FILE* output) {
    for (int i = 0; i < mst_edge_count; i++) {
        fprintf(output, "(%d,%d) ", mst[i].source + 1, mst[i].destination + 1);
    }
    fprintf(output, "\n");
}

int main(int argc, char* argv[]) {
    char *input_file = NULL, *output_file = NULL;
    int show_solution = 0;
    int start_vertex = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            show_help();
            return 0;
        } 
        else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                input_file = argv[++i];
            } 
            else {
                fprintf(stderr, "Error: -f requires a filename\n");
                return 1;
            }
        } 
        else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } 
            else {
                fprintf(stderr, "Error: -o requires a filename\n");
                return 1;
            }
        } 
        else if (strcmp(argv[i], "-i") == 0) { 
            if (i + 1 < argc) {
                start_vertex = atoi(argv[++i]) - 1;
            } 
            else {
                fprintf(stderr, "Error: -i requires a vertex number\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-s") == 0) {
            show_solution = 1;
        } 
        else {
            fprintf(stderr, "Invalid option: %s. Use -h for help\n", argv[i]);
            return 1;
        }
    }

    if (!input_file) {
        fprintf(stderr, "Required parameters missing. Use -h for help\n");
        return 1;
    }

    int vertex_count, file_edge_count;
    load_edges(input_file, &vertex_count, &file_edge_count);

    if (start_vertex != -1 && vertex_count > 0 && 
        (start_vertex < 0 || start_vertex >= vertex_count)) {
        fprintf(stderr, "Warning: Start vertex (%d) out of range for graph with %d vertices. Ignoring for Kruskal.\n", 
                start_vertex + 1, vertex_count);
    }

    int mst_edge_count = 0;
    Edge* mst = find_mst(vertex_count, &mst_edge_count);

    FILE* output = stdout;
    if (output_file) {
        output = fopen(output_file, "w");
        if (!output) {
            perror("Error opening output file");
            free(mst);
            return 1;
        }
    }

    if (show_solution) {
        print_mst(mst, mst_edge_count, output);
    }
    else {
        int total_weight = calculate_mst_weight(mst, mst_edge_count);
        fprintf(output, "%d\n", total_weight);
    }

    free(mst);
    if (output_file && output != stdout) fclose(output);

    return 0;
}
