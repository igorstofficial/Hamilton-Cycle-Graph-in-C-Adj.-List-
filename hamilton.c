#include "graphFuncs.h"
#include <stdio.h>
#include <stdlib.h>

//function to check if vertex is visited
int isVisited(int* visited, int vertex, int size)
{
	for (int i = 0; i < size; i++) {
		if (visited[i] == vertex)
			return 1;
	}

	return 0;
}

//func to return pointer to vertex X
EdgeNode* getVertex(Graph* g, int vertex)
{
	EdgeNode* temp = g->edges[0]; //temp pointer

	for (int i = 0; i < g->verticesAmount;) {
		//if x in temp equal to vertex it's correct pointer
		if (temp->x == vertex)
			return temp;
		else temp = g->edges[++i];
	}

	return NULL; //if not found
}

//function to solute graph connection problem
void connectivity(Graph* g, int vertex, int* visited, int size)
{
	static int counter = 0; //counter of visited

	visited[counter++] = vertex; // mark current vertex as visited

	EdgeNode* temp;
	temp = getVertex(g, vertex); //temp pointer to current vertex's connections

	//while vertex has connections
	for (int i = 0; temp != NULL; temp = temp->next) {
		i = temp->y;
		if (!isVisited(visited, i, size))//if unvisited, recurse function with new vertex [i]
			connectivity(g, i, visited, size);
	}
}

//function to show if graph is connected
int isConnected(Graph* g)
{
	//visited -> array to hold 0 if vertex isn't visited, 1 - if visited. size -> amount of vertices
	int* visited = (int*)malloc(sizeof(int)* g->verticesAmount); int size = g->verticesAmount;
	for (int i = 0; i < size; ++i) visited[i] = -1; //all vertices are unvisited yet

	int startVertex = g->edges[0]->x;

	//check for connectivity
	connectivity(g, startVertex, visited, size);

	int visitedCounter = 0; //variable to count how many vertices are visited

	for (int i = 0; i < size; i++)
		if (isVisited(visited, g->edges[i]->x, size)) //if vertex is visited - increase counter
			visitedCounter++;

	if (visitedCounter != size) // if amount of visited vertices aren't equal to their true amount - disconnected
		return 0;

	free(visited);//deleting memory allocated for visited

	return 1; // graph is connected

}

//function to checking if this Hamilton Path is Cycle or just a path
int isCycle(Graph* g, int currentVertex, int startVertex)
{
	EdgeNode* temp = getVertex(g, currentVertex);//points to the last vertex

	int isCycle = 0; //variable to show if it is cycle[1 - yes, 0 - no]

	while (temp != NULL) { //while there's any vertex connected

		if (temp->y == startVertex) { //if this vertex is connected with start vertex - that's cycle
			isCycle = 1;
			break;
		}
		else temp = temp->next;

	}

	return isCycle;
}

//function to return edge's weight
int getWeight(Graph* g, int startVertex, int endVertex)
{
	EdgeNode* temp = getVertex(g, startVertex); //temporary pointer

	//while has connected vertices
	while (temp != NULL) {
		if (temp->y == endVertex) //if vertex is the same - break
			break;
		else temp = temp->next;
	}

	return temp->weight; 

}

//funtion to print Hamilton Cycle
void printCycle(Graph* g, int* fullPath, int size)
{
	printf("\nHamilton Cycle(x [weight] y): ");

	for (int i = 0; i < size; i++) {
		printf("%d ", fullPath[i]);

		//if vertex is prelast - break
		if (i == size - 1)
			break;

		//print weight between current and next vertex
		printf("[%d] ", getWeight(g, fullPath[i], fullPath[i + 1]));
	}

	//print weight between prelast and last vertex
	printf("[%d] ", getWeight(g, fullPath[size - 1], fullPath[0]));

	//print last vertex
	printf("%d\n", fullPath[0]);
}

//funtion that solutes the Hamilton Cycle problem
void HamiltonCycle(Graph* g, int* path, int* visited, int size, int vertex, int* existCycle)
{
	static unsigned counter = 1; //counter of visited vertices

	//base for recurtion
	if (counter == size) { //if all the vertices are visited
		if (isCycle(g, vertex, path[0])) { //check if this is cycle
			printCycle(g, path, size); *existCycle = 1;//cycle found
			return;
		}

		else return;
	}

	EdgeNode* temp; // temporary pointer
	temp = getVertex(g, vertex); //points to the current vertex to see then all the vertices connected with it

	//while there's any connected vertex yet and cycle is unfound
	for (int i = 0; temp != NULL && !(*existCycle); temp = temp->next) {
		i = temp->y; //i = vertex connected with current

		if (!isVisited(visited, i, size)) { //if vertex unvisited
			visited[counter] = i; //mark it visited
			path[counter++] = i; //increase the path adding this vertex

			HamiltonCycle(g, path, visited, size, i, existCycle); //recurse the function with this unvisited early vertex [i]
			
			//backtracking
			visited[--counter] = 0; //if it didn't lead to cycle, deleting the last vertex from visited
			path[counter] = -1; // and decreasing the path's counter
		}

	}

}


//function to check if there's such a vertex in graph
int doesVertexExist(Graph* g, int vertex)
{
	return (getVertex(g, vertex) != NULL) ? 1 : 0; //if pointer that points to vertex's num isn't NULL - exists
}

//function to start finding the cycle, input needed data(vertex)
void findHamiltonCycle(Graph* g)
{

	int vertex; //vertex we are looking HC for

	int mode = 0;//variable for input validation

	printf("\nPlease enter the vertex you want to find Hamilton Cycle for: ");

	do {

		if ((mode = scanf_s("%d", &vertex)) == 0 || doesVertexExist(g, vertex) == 0) {
			printf("\nInvalid input! Such a vertex doesn't exist in the Graph! \nTry again: ");  mode = 0; getchar();
		}

	} while (!mode); mode = 0; //if vertex doesn't exist we repeat


	int* path = (int*)malloc(sizeof(int)* g->verticesAmount);//array to hold Hamilton Path

	path[0] = vertex; //path starts from vertex we are looking the path for

	int size = g->verticesAmount; //size - vertices amount to go through

	int* visited = (int*)malloc(sizeof(int)* g->verticesAmount); // array to hold 1 if vertex is visited 0 if unvisited

	//initializing the array with default 0
	for (int i = 0; i < g->verticesAmount; i++) {
		visited[i] = -1;
	} visited[0] = vertex; //currect vertex is visited

	int existCycle = 0; //variable to check if any cycle has been found

	//finding solution, if there's any, it will be printed, otherwise - not
	HamiltonCycle(g, path, visited, size, vertex, &existCycle);

	//if path is empty - no cycle
	if (!existCycle)
		printf("\nThere's no Hamilton Cycle for this vertex!\n");

	free(path);
	free(visited);

}
