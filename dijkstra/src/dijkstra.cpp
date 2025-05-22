#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>

using namespace std;

struct Edge {
    int dest;
    int weight;
};

void dijkstra(const vector<vector<Edge>>& graph, int start, unordered_map<int, int>& distances) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        int dist_u = pq.top().first;
        pq.pop();

        if (dist_u > distances[u]) continue;

        for (const Edge& e : graph[u]) {
            int v = e.dest;
            int weight = e.weight;

            if (distances.find(v) == distances.end() || distances[v] > distances[u] + weight) {
                distances[v] = distances[u] + weight;
                pq.push({distances[v], v});
            }
        }
    }
}

int main(int argc, char* argv[]) {
    string input_file, output_file;
    int start_vertex = -1;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-h") {
            cout << "Uso: " << argv[0] << " [OPCOES]\n"
                 << "Opcoes:\n"
                 << "-h : Mostra ajuda\n"
                 << "-o <arquivo> : Redireciona saida para arquivo\n"
                 << "-f <arquivo> : Arquivo de entrada\n"
                 << "-i : Vertice inicial\n";
            return 0;
        } else if (arg == "-f" && i + 1 < argc) {
            input_file = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else if (arg == "-i" && i + 1 < argc) {
            start_vertex = stoi(argv[++i]);
        }
    }

    if (input_file.empty() || start_vertex == -1) {
        cerr << "Erro: Argumentos faltando. Use -h para ajuda.\n";
        return 1;
    }

    ifstream fin(input_file);
    if (!fin) {
        cerr << "Erro: Nao foi possivel abrir " << input_file << "\n";
        return 1;
    }

    int V, E;
    fin >> V >> E;

    vector<vector<Edge>> graph(V + 1);

    for (int i = 0; i < E; ++i) {
        int u, v, w;
        fin >> u >> v >> w;
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    unordered_map<int, int> distances;
    dijkstra(graph, start_vertex, distances);

    ostream* out = &cout;
    ofstream fout;

    if (!output_file.empty()) {
        fout.open(output_file);
        if (!fout) {
            cerr << "Erro: Nao foi possivel criar " << output_file << "\n";
            return 1;
        }
        out = &fout;
    }

    for (int v = 1; v <= V; ++v) {
        if (v != 1) *out << " ";
        *out << v << ":";
        if (distances.find(v) != distances.end()) {
            *out << distances[v];
        } else {
            *out << "-1";
        }
    }
    *out << "\n";

    if (fout.is_open()) {
        fout.close();
    }

    return 0;
}