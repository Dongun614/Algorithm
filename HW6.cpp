// homework6.cpp
// References:
// (1) 강의 slide chapter 16.
// (2) Book: “Algorithm Design Manual” by Steven S. Skiena

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <queue>
#include <iomanip>

using namespace std;

const int INF = 1e9;
const int MAX_V = 20;

int numVertices;
vector<string> cityNames;
vector<vector<int>> adjMatrix;

void readGraph(const string &filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<int> row;

        // 도시 이름 읽기
        getline(ss, token, '\t');
        cityNames.push_back(token);

        // 거리값 읽기
        while (getline(ss, token, '\t')) {
            // trim
            token.erase(0, token.find_first_not_of(" \r\n"));
            token.erase(token.find_last_not_of(" \r\n") + 1);

            if (token == "INF") {
                row.push_back(INF);
            } else {
                try {
                    row.push_back(stoi(token));
                } catch (...) {
                    cerr << "잘못된 거리값: '" << token << "'\n";
                    row.push_back(INF);
                }
            }
        }
        adjMatrix.push_back(row);
    }

    numVertices = adjMatrix.size();
}

void printTable(const vector<vector<int>>& dist) {
    cout << "\t";
    for (const auto& name : cityNames) {
        cout << name << "\t";
    }
    cout << "\n";

    for (int i = 0; i < numVertices; ++i) {
        cout << cityNames[i] << "\t";
        for (int j = 0; j < numVertices; ++j) {
            if (dist[i][j] == INF) cout << "INF\t";
            else cout << dist[i][j] << "\t";
        }
        cout << "\n";
    }
}

void dijkstraAllPairs() {
    cout << "1) The shortest distance between cities using Dijkstra's algorithm is:\n";

    vector<vector<int>> allDist(numVertices, vector<int>(numVertices, INF));

    for (int src = 0; src < numVertices; ++src) {
        vector<int> dist(numVertices, INF);
        dist[src] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;

            for (int v = 0; v < numVertices; ++v) {
                if (adjMatrix[u][v] != INF && dist[v] > dist[u] + adjMatrix[u][v]) {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    pq.push({dist[v], v});
                }
            }
        }
        allDist[src] = dist;
    }

    printTable(allDist);
}

void floydWarshall() {
    cout << "\n2) The shortest distance between cities using Floyd's algorithm is:\n";
    vector<vector<int>> dist = adjMatrix;

    for (int k = 0; k < numVertices; ++k) {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    printTable(dist);
}

int main() {
    readGraph("homework6.data");

    cout << fixed << setprecision(0);
    dijkstraAllPairs();
    floydWarshall();

    return 0;
}
