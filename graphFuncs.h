typedef struct {
	int x;//start point
	int y;//endination ( x -> y. y is endination)
	int weight;//edge weight
	struct EdgeNode* next;//pointer to vertex that is connected with current
}EdgeNode; // structure for one Edge

typedef struct {
	EdgeNode** edges;//adjacency list
	int isDirected;//is directed, 0 - no, 1 - yes
	int verticesAmount;//actual vertices amount
	int size;//amount of vertices given by user
}Graph; // structure for the whole Graph

//check if graph is directed
int isDirected(Graph* g);

//function to check if vertex is visited
int isVisited(int* visited, int vertex, int size);

//func to return pointer to vertex X
EdgeNode* getVertex(Graph* g, int vertex);

//function to choose the variant of graph's initialization
int inputGraph(Graph* g);

//initializing an empty graph
void initializeGraph(Graph *g, int isDirected, int size);

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

//function to return edge's weight
int getWeight(Graph* g, int startVertex, int endVertex);

//funtion to print Hamilton Cycle
void printCycle(Graph* g, int* fullPath, int size);

//funtion that solutes the Hamilton Cycle problem
void HamiltonCycle(Graph* g, int* path, int* visited, int size, int vertex, int* existCycle);

//function to check if there's such a vertex in graph
int doesVertexExist(Graph* g, int vertex);

//function to start finding the cycle, input needed data(vertex)
void findHamiltonCycle(Graph* g);

//funtion to initialize the graph from the file, returns 1 if successful, otherwise - 0
int readGraphFromFile(Graph* g);

//function to solute graph connection problem
void connectivity(Graph* g, int vertex, int* visited, int size);

//function to show if graph is connected
int isConnected(Graph* g);