#include <stdio.h>
#include <stdlib.h>
#include "../headers/kruskal.h"

#define MAX_VERTICES 1000

int parent[MAX_VERTICES];
int tree_rank[MAX_VERTICES];

Edge edges[MAX_VERTICES * MAX_VERTICES];
int edge_count = 0;

void add_edge(int source, int destination, int weight) {
    edges[edge_count].source = source;
    edges[edge_count].destination = destination;
    edges[edge_count].weight = weight;
    edge_count++;
}

int compare_edges(const void* a, const void* b) {
    Edge* edge1 = (Edge*)a;
    Edge* edge2 = (Edge*)b;
    return edge1->weight - edge2->weight;
}

void initialize_sets(int vertex_count) {
    for(int i = 0; i < vertex_count; i++) {
        parent[i] = i;
        tree_rank[i] = 0;
    }
}

int find_set(int vertex) {
    if (parent[vertex] != vertex)
        parent[vertex] = find_set(parent[vertex]);
    return parent[vertex];
}

void union_sets(int u, int v) {
    int root_u = find_set(u);
    int root_v = find_set(v);
    
    if (root_u != root_v) {
        if (tree_rank[root_u] < tree_rank[root_v]) {
            parent[root_u] = root_v;
        } else if (tree_rank[root_u] > tree_rank[root_v]) {
            parent[root_v] = root_u;
        } else {
            parent[root_v] = root_u;
            tree_rank[root_u]++;
        }
    }
}

Edge* find_mst(int vertex_count, int* mst_edge_count) {
    qsort(edges, edge_count, sizeof(Edge), compare_edges);
    initialize_sets(vertex_count);

    Edge* mst = malloc((vertex_count - 1) * sizeof(Edge));
    int mst_index = 0;

    for(int i = 0; i < edge_count && mst_index < (vertex_count - 1); i++) {
        if(find_set(edges[i].source) != find_set(edges[i].destination)) {
            mst[mst_index++] = edges[i];
            union_sets(edges[i].source, edges[i].destination);
        }
    }
    
    *mst_edge_count = mst_index;
    return mst;
}

int calculate_mst_weight(Edge* mst_edges, int edge_count) {
    int total_weight = 0;
    for(int i = 0; i < edge_count; i++) {
        total_weight += mst_edges[i].weight;
    }
    return total_weight;
}

int get_edge_count() {
    return edge_count;
}