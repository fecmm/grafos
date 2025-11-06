#ifndef KRUSKAL_H
#define KRUSKAL_H

#define MAX_VERTICES 1000


typedef struct {
    int source;
    int destination;
    int weight;
} Edge;

extern int parent[MAX_VERTICES];
extern int tree_rank[MAX_VERTICES];
extern Edge edges[MAX_VERTICES * MAX_VERTICES];
extern int edge_count;

void add_edge(int source, int dest, int weight);
int compare_edges(const void* a, const void* b);
void init_sets(int vertex_count);
int find_set(int vertex);
void union_sets(int u, int v);
Edge* kruskal_mst(int vertex_count, int* mst_size);
int total_mst_weight(Edge* mst, int size);
int get_edge_count(void);

#endif
