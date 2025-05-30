/*
22000712 조동운

References:
(1) 강의 슬라이드 - 7. ch22
(2) https://blog.naver.com/ndb796/221227975229
(3) https://itguava.tistory.com/67
(4) https://m42-orion.tistory.com/65#%E2%AD%90%EF%B8%8F%20Code-1
(5) https://better-tomorrow.tistory.com/entry/DFS-BFS-%EC%9D%B4%ED%95%B4%ED%95%98%EA%B8%B0
(6) https://m42-orion.tistory.com/63
(7) https://wannadev.tistory.com/96
*/

#include <iostream>
#include <algorithm>

using namespace std;

#define MAX_NODES 10 

typedef enum {
    WHITE, 
    GRAY,  
    BLACK
} COLOR;

typedef struct AdjListNode {
    char label;
    struct AdjListNode* next;
} AdjListNode;

typedef struct Graph {
    char nodeLabels[MAX_NODES]; 
    AdjListNode* adj[MAX_NODES]; 
    int numVertices;
} Graph;

Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    for (int i = 0; i < MAX_NODES; i++) {
        graph->adj[i] = NULL;
        graph->nodeLabels[i] = '\0'; 
    }
    graph->numVertices = 0;
    return graph;
}

void addVertex(Graph* graph, char label) {
    if (graph->numVertices < MAX_NODES) {
        for (int i = 0; i < graph->numVertices; i++) {
            if (graph->nodeLabels[i] == label) {
                printf("Vertex %c already exists.\n", label);
                return;
            }
        }
        graph->nodeLabels[graph->numVertices] = label;
        graph->numVertices++;
    } else {
        printf("Maximum number of nodes reached.\n");
    }
}

int getVertexIndex(Graph* graph, char label) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->nodeLabels[i] == label) {
            //printf("index is %d\n", i);
            return i;
        }
    }
    return -1;
}

// Function to add an edge
void addEdge(Graph* graph, char srcLabel, char destLabel) {
    int srcIndex = getVertexIndex(graph, srcLabel);
    int destIndex = getVertexIndex(graph, destLabel);

    if (srcIndex == -1 || destIndex == -1) {
        printf("Invalid vertex label.\n");
        return;
    }

    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->label = destLabel;
    newNode->next = graph->adj[srcIndex];
    graph->adj[srcIndex] = newNode;
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < MAX_NODES; i++) {
        AdjListNode* current = graph->adj[i];
        while (current != NULL) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

void printGraph(Graph* graph) {
    printf("Adjacency List:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%c: ", graph->nodeLabels[i]);
        AdjListNode* current = graph->adj[i];
        while (current != NULL) {
            printf("%c -> ", current->label);
            current = current->next;
        }
        printf("NULL\n");
    }
}

void DFS_VISIT(Graph* graph, int u, COLOR color[], int* time, int d[], int f[]) {
    color[u] = GRAY;
    (*time)++;
    d[u] = *time;

    AdjListNode* current = graph->adj[u];
    while (current != NULL) {
        int v = getVertexIndex(graph, current->label);
        if (v != -1 && color[v] == WHITE) {
            DFS_VISIT(graph, v, color, time, d, f);
        }
        current = current->next;
    }

    color[u] = BLACK;
    (*time)++;
    f[u] = *time;
}


void DFS(Graph* graph, COLOR color[], int* time, int d[], int f[]) {
    for (int i = 0; i < graph->numVertices; i++) {
        color[i] = WHITE;
        d[i] = -1;
        f[i] = -1;
    }
    *time = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (color[i] == WHITE) {
            DFS_VISIT(graph, i, color, time, d, f);
        }
    }
}

bool compare(const pair<char, int>& a, const pair<char, int>& b) {
    return a.second > b.second; 
}


int main() {
    int size;
    Graph* graph = createGraph();
    printf("Enter the number of nodes: ");
    scanf(" %d", &size);

    for (int i = 0; i < size; i++) {
        addVertex(graph, 'A' + i);
    }

    addEdge(graph, 'A', 'H');
    addEdge(graph, 'B', 'A');
    addEdge(graph, 'B', 'D');
    addEdge(graph, 'B', 'F');
    addEdge(graph, 'C', 'D');
    addEdge(graph, 'C', 'F');
    addEdge(graph, 'D', 'A');
    addEdge(graph, 'D', 'G');
    addEdge(graph, 'D', 'H');
    addEdge(graph, 'D', 'I');
    addEdge(graph, 'E', 'I');

    printGraph(graph);
    printf("\n");

    COLOR color[MAX_NODES];
    int time = -1;
    int d[MAX_NODES]; 
    int f[MAX_NODES]; 

    DFS(graph, color, &time, d, f);

    printf("\nDFS Results:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %c: Discovery Time = %d, Finishing Time = %d, Color = ",
               graph->nodeLabels[i], d[i], f[i]);
        if (color[i] == WHITE) printf("WHITE");
        else if (color[i] == GRAY) printf("GRAY");
        else if (color[i] == BLACK) printf("BLACK");
        printf("\n");
    }

    vector<pair<char, int>> nodes;
    for (int i = 0; i < graph->numVertices; ++i) {
        nodes.push_back({graph->nodeLabels[i], f[i]});
    }

    printf("\nTopological Results:\n");
    sort(nodes.begin(), nodes.end(), compare);
    for(auto& i : nodes ){
        printf("%c ", i.first);
    }
    printf("\n");

    freeGraph(graph);

    return 0;
}