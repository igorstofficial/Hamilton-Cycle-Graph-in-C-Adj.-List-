#define MAX 300 // max amount of vertices

typedef struct {
	int end;//endination
	int weight;//edge weight
	struct EdgeNode* next;//pointer to vertex that is connected with current
}EdgeNode; // structure for one Edge

typedef struct {
	EdgeNode* edges[MAX];//adjacency list
	int verticesAmount;//amount of vertices
	int edgesAmount;//amount of edges
	int isDirected;//is directed, 0 - no, 1 - yes
}Graph; // structure for the whole Graph

//function to choose the variant of graph's initialization
int inputGraph(Graph* g);

//initializing an empty graph
void initializeGraph(Graph *g, int isDirected);

//initializing the edge of the graph
void insertEdge(Graph *g, int x, int y, int weight, int isDirected);

//inputing the graph from keyboard
void readGraph(Graph *g);

//printing the graph out
void printGraph(Graph* g);

//deleting memory allocated for the graph
void freeGraph(Graph* g);

//function to checking if this Hamilton Path is Cycle or just a path
int isCycle(Graph* g, int currentVertex, int startVertex);

//funtion to print Hamilton Cycle
void printCycle(int* fullPath, int size);

//funtion that solutes the Hamilton Cycle problem
void HamiltonCycle(Graph* g, int* path, int* visited, int size, int vertex, int* existCycle);

//function to check if there's such a vertex in graph
int doesVertexExist(Graph* g, int vertex);

//function to start finding the cycle, input needed data(vertex)
void findHamiltonCycle(Graph* g);

//funtion to initialize the graph from the file, returns 1 if successful, otherwise - 0
int readGraphFromFile(Graph* g);

//function to solute graph connection problem
void connectivity(Graph* g, int vertex, int* visited);

//function to show if graph is connected
int isConnected(Graph* g);
