#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;


void dfs(int v, vector<bool>& visited, vector<vector<int>>& adj, stack<int>& st) {
    visited[v] = true;
    for (int u : adj[v]) {
        if (!visited[u])
            dfs(u, visited, adj, st);
    }
    st.push(v);
}

void dfs_reverse(int v, vector<bool>& visited, vector<vector<int>>& adj_rev, vector<int>& component) {
    visited[v] = true;
    component.push_back(v+1);
    for (int u : adj_rev[v]) {
        if (!visited[u])
            dfs_reverse(u, visited, adj_rev, component);
    }
}

int main(int argc, char* argv[]) {
    string input_file, output_file;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-h") {
            cout << "Uso: " << argv[0] << " -f <arquivo> [-o <saida>]\n";
            return 0;
        } else if (arg == "-f") input_file = argv[++i];
        else if (arg == "-o") output_file = argv[++i];
    }

    ifstream fin(input_file);
    int n, m;
    fin >> n >> m;

    vector<vector<int>> adj(n), adj_rev(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        fin >> u >> v;
        adj[u-1].push_back(v-1);
        adj_rev[v-1].push_back(u-1);
    }

    stack<int> st;
    vector<bool> visited(n, false);

    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfs(i, visited, adj, st);
    }

    fill(visited.begin(), visited.end(), false);
    vector<vector<int>> components;

    while (!st.empty()) {
        int v = st.top();
        st.pop();

        if (!visited[v]) {
            vector<int> component;
            dfs_reverse(v, visited, adj_rev, component);
            components.push_back(component);
        }
    }

    ofstream fout(output_file);
    ostream& out = output_file.empty() ? cout : fout;

    for (auto& component : components) {
        for (int v : component)
            out << v << " ";
        out << "\n";
    }

    return 0;
}
