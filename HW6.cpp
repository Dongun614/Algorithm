// homework6.cpp
// References:
// (1) 강의 slide: chapter 24-25
// (2) https://velog.io/@gks970113-woo/CC-stringstream-%EC%82%AC%EC%9A%A9%EB%B2%95
// (3) https://yabmoons.tistory.com/364
// (4) https://ansohxxn.github.io/algorithm/floyd/
// (5) https://musket-ade.tistory.com/entry/C-%EC%98%88%EC%99%B8%EC%B2%98%EB%A6%AC-%EB%A9%94%EC%BB%A4%EB%8B%88%EC%A6%98-try-catch-throw-%EC%B4%9D-%EC%A0%95%EB%A6%AC
// (6) https://huilife.tistory.com/20

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <queue>
#include <iomanip>

using namespace std;

const int INF = INT_MAX;
const int MAX_VERTICE = 20;

int numVertices;
vector<string> city;
vector<vector<int>> matrix;

void readFile(const string &filename);
void printFormat(const vector<vector<int>>& dist);
void dijkstra();
void floydWarshall();

int main() {
    readFile("homework6.data");

    cout << fixed << setprecision(0);
    dijkstra();
    floydWarshall();

    return 0;
}

void readFile(const string &filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<int> row;

        getline(ss, token, '\t');
        city.push_back(token);

        while (getline(ss, token, '\t')) {
            token.erase(0, token.find_first_not_of(" \r\n"));
            token.erase(token.find_last_not_of(" \r\n") + 1);

            if (token == "INF") {
                row.push_back(INF);
            } else {
                try {
                    row.push_back(stoi(token));
                } catch (...) {
                    cerr << "잘못된 거리값입니다\n";
                    row.push_back(INF);
                }
            }
        }
        matrix.push_back(row);
    }

    numVertices = matrix.size();
}

void printFormat(const vector<vector<int>>& dist) {
    const int width = 10;

    cout << left << setw(width) << "";
    for (const auto& name : city) {
        cout << left << setw(width) << name;
    }
    cout << "\n";

    for (int i = 0; i < numVertices; ++i) {
        cout << left << setw(width) << city[i];
        for (int j = 0; j < numVertices; ++j) {
            if (dist[i][j] == INF)
                cout << left << setw(width) << "INF";
            else
                cout << left << setw(width) << dist[i][j];
        }
        cout << "\n";
    }
}


void dijkstra() {
    cout << "1) Dijkstra's algorithm:\n\n";

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
                if (matrix[u][v] != INF && dist[v] > dist[u] + matrix[u][v]) {
                    dist[v] = dist[u] + matrix[u][v];
                    pq.push({dist[v], v});
                }
            }
        }
        allDist[src] = dist;
    }

    printFormat(allDist);
}

void floydWarshall() {
    cout << "\n2) Floyd-Warshall algorithm:\n\n";
    vector<vector<int>> dist = matrix;

    for (int k = 0; k < numVertices; ++k) {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    printFormat(dist);
}
