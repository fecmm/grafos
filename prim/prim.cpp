#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

int main(int argc, char* argv[]) {
    string input_file, output_file;
    int start_vertex = 0;
    bool show_solution = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-h") {
            cout << "Uso: " << argv[0] << " -f <arquivo> -i <vertice> [-s] [-o <saida>]\n";
            return 0;
        } else if (arg == "-f") input_file = argv[++i];
        else if (arg == "-i") start_vertex = stoi(argv[++i]) - 1;
        else if (arg == "-s") show_solution = true;
        else if (arg == "-o") output_file = argv[++i];
    }

    ifstream fin(input_file);
    int n, m;
    fin >> n >> m;

    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        fin >> u >> v >> w;
        adj[u-1].emplace_back(v-1, w);
        adj[v-1].emplace_back(u-1, w);
    }

    priority_queue<pair<int, pair<int, int>>> pq;
    vector<bool> in_mst(n, false);
    vector<pair<int, int>> mst_edges;
    int cost = 0;

    in_mst[start_vertex] = true;
    for (auto edge : adj[start_vertex])
        pq.push({-edge.second, {start_vertex, edge.first}});

    while (!pq.empty()) {
        auto edge = pq.top();
        pq.pop();
        int w = -edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        if (in_mst[v]) continue;

        in_mst[v] = true;
        cost += w;
        mst_edges.emplace_back(u+1, v+1);

        for (auto next : adj[v]) {
            if (!in_mst[next.first])
                pq.push({-next.second, {v, next.first}});
        }
    }

    ofstream fout(output_file);
    ostream& out = output_file.empty() ? cout : fout;

    if (show_solution) {
        for (auto e : mst_edges) 
            out << "(" << e.first << "," << e.second << ") ";
        out << endl;
    } else {
        out << cost << endl;
    }

    return 0;
}