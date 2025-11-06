#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, weight;
    bool operator<(Edge const& other) {
        return weight < other.weight;
    }
};


vector<int> parent, rank_;

void make_set(int v) {
    parent[v] = v;
    rank_[v] = 0;
}

int find_set(int v) {
    if (v == parent[v]) return v;
    return parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank_[a] < rank_[b]) swap(a, b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;
    }
}

int main(int argc, char* argv[]) {
    string input_file, output_file;
    bool show_solution = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-h") {
            cout << "Uso: " << argv[0] << " -f <arquivo> [-s] [-o <saida>]\n";
            return 0;
        } else if (arg == "-f") input_file = argv[++i];
        else if (arg == "-s") show_solution = true;
        else if (arg == "-o") output_file = argv[++i];
    }

    ifstream fin(input_file);
    int n, m;
    fin >> n >> m;

    vector<Edge> edges;
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        fin >> u >> v >> w;
        edges.push_back({u-1, v-1, w});
    }

    parent.resize(n);
    rank_.resize(n);
    for (int i = 0; i < n; i++) make_set(i);

    sort(edges.begin(), edges.end());

    int cost = 0;
    vector<Edge> result;
    for (Edge e : edges) {
        if (find_set(e.u) != find_set(e.v)) {
            cost += e.weight;
            result.push_back(e);
            union_sets(e.u, e.v);
        }
    }

    ofstream fout(output_file);
    ostream& out = output_file.empty() ? cout : fout;

    if (show_solution) {
        for (Edge e : result) 
            out << "(" << e.u+1 << "," << e.v+1 << ") ";
        out << endl;
    } else {
        out << cost << endl;
    }

    return 0;
}
