#include <iostream>

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
    *time = -1;
    for (int i = 0; i < graph->numVertices; i++) {
        if (color[i] == WHITE) {
            DFS_VISIT(graph, i, color, time, d, f);
        }
    }
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


    freeGraph(graph);

    return 0;
}