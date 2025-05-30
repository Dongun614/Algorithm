// Lecture slide chapter 16.
// Book: "Algorithm analysis in C++" by Mark Allen Weiss

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
#include <string>

using namespace std;

class Graph {
private:
    map<string, vector<string>> adj; // 인접 리스트 (알파벳 순서 보장)
    map<string, bool> visited;
    map<string, int> startTime;
    map<string, int> finishTime;
    vector<string> topoOrder;
    int time;

public:
    Graph() : time(0) {}

    void addVertex(const string& v) {
        if (adj.find(v) == adj.end()) {
            adj[v] = vector<string>();
        }
    }

    void addEdge(const string& u, const string& v) {
        addVertex(u);
        addVertex(v);
        adj[u].push_back(v);
        sort(adj[u].begin(), adj[u].end()); // 알파벳 순서 정렬
    }

    void dfsAll() {
        visited.clear();
        startTime.clear();
        finishTime.clear();
        topoOrder.clear();
        time = 0;

        for (const auto& pair : adj) {
            visited[pair.first] = false;
        }

        for (const auto& pair : adj) {
            if (!visited[pair.first]) {
                dfs(pair.first);
            }
        }
    }

    void dfs(const string& u) {
        visited[u] = true;
        startTime[u] = ++time;
        for (const string& v : adj[u]) {
            if (!visited[v]) {
                dfs(v);
            }
        }
        finishTime[u] = ++time;
        topoOrder.push_back(u); // 종료 시 스택에 push
    }

    void printTimes() const {
        cout << "Vertex\tStart\tFinish\n";
        for (const auto& pair : startTime) {
            cout << pair.first << "\t" << pair.second << "\t" << finishTime.at(pair.first) << "\n";
        }
    }

    void printTopologicalOrder() const {
        cout << "Topological Ordering: ";
        for (auto it = topoOrder.rbegin(); it != topoOrder.rend(); ++it) {
            cout << *it;
            if (it + 1 != topoOrder.rend()) cout << " -> ";
        }
        cout << endl;
    }
};

int main() {
    Graph g;

    // 예시 그래프 (이미지 참고, A~F)
    g.addEdge("A", "C");
    g.addEdge("A", "D");
    g.addEdge("B", "D");
    g.addEdge("B", "E");
    g.addEdge("C", "F");
    g.addEdge("D", "F");
    g.addEdge("E", "F");

    // DFS 및 위상 정렬
    g.dfsAll();

    // 결과 출력
    cout << "=== DFS Start/Finish Times ===\n";
    g.printTimes();
    cout << "\n=== Topological Sort ===\n";
    g.printTopologicalOrder();

    return 0;
}
