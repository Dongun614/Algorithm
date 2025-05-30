#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <stdexcept>
using namespace std;

enum class Color { WHITE, GRAY, BLACK };

class Graph {
private:
    vector<char> nodeLabels;
    map<char, int> labelIndex;
    vector<list<char>> adj;
    vector<Color> color;
    vector<int> discoveryTime;
    vector<int> finishTime;
    int time;

    void DFS_VISIT(int u) {
        color[u] = Color::GRAY;
        discoveryTime[u] = ++time;

        for (char v_label : adj[u]) {
            auto it = labelIndex.find(v_label);
            if (it == labelIndex.end()) continue;
            
            int v = it->second;
            if (color[v] == Color::WHITE) {
                DFS_VISIT(v);
            }
        }

        color[u] = Color::BLACK;
        finishTime[u] = ++time;
    }

public:
    Graph() : time(0) {}

    void addVertex(char label) {
        if (labelIndex.count(label)) {
            cerr << "Vertex " << label << " already exists." << endl;
            return;
        }
        nodeLabels.push_back(label);
        adj.push_back(list<char>());
        labelIndex[label] = nodeLabels.size() - 1;
    }

    void addEdge(char src, char dest) {
        if (!labelIndex.count(src) || !labelIndex.count(dest)) {
            cerr << "Invalid edge: " << src << "->" << dest << endl;
            return;
        }
        adj[labelIndex[src]].push_front(dest);
    }

    void DFS() {
        color.assign(nodeLabels.size(), Color::WHITE);
        discoveryTime.assign(nodeLabels.size(), -1);
        finishTime.assign(nodeLabels.size(), -1);
        time = 0;

        for (int i = 0; i < nodeLabels.size(); ++i) {
            if (color[i] == Color::WHITE) {
                DFS_VISIT(i);
            }
        }
    }

    void printGraph() const {
        cout << "Adjacency List:\n";
        for (size_t i = 0; i < nodeLabels.size(); ++i) {
            cout << nodeLabels[i] << ": ";
            for (char neighbor : adj[i]) {
                cout << neighbor << " -> ";
            }
            cout << "NULL\n";
        }
    }

    // Getter methods for DFS results
    int getDiscoveryTime(char label) const {
        if (!labelIndex.count(label)) throw invalid_argument("Invalid label");
        return discoveryTime[labelIndex.at(label)];
    }

    int getFinishTime(char label) const {
        if (!labelIndex.count(label)) throw invalid_argument("Invalid label");
        return finishTime[labelIndex.at(label)];
    }

    vector<char> getLabels() const { return nodeLabels; }
};

int main() {
    Graph g;
    int size;

    cout << "Enter the number of nodes: ";
    cin >> size;

    for (int i = 0; i < size; ++i) {
        g.addVertex('A' + i);
    }

    // Add edges from original C code example
    g.addEdge('A', 'H');
    g.addEdge('B', 'A');
    g.addEdge('B', 'D');
    g.addEdge('B', 'F');
    g.addEdge('C', 'D');
    g.addEdge('C', 'F');
    g.addEdge('D', 'A');
    g.addEdge('D', 'G');
    g.addEdge('D', 'H');
    g.addEdge('D', 'I');
    g.addEdge('E', 'I');

    g.printGraph();
    g.DFS();

    cout << "\nDFS Results:\n";
    for (char label : g.getLabels()) {
        cout << "Vertex " << label << ": "
             << "Discovery=" << g.getDiscoveryTime(label)
             << ", Finish=" << g.getFinishTime(label) << endl;
    }

    return 0;
}
