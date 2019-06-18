#include "graphFuncs.h"
#include <stdio.h>
#include <stdlib.h>

//function to solute graph connection problem
void connectivity(Graph* g, int vertex, int* visited)
{
	visited[vertex] = 1; // mark current vertex as visited

	EdgeNode* temp;
	temp = g->edges[vertex]; //temp pointer to current vertex's connections

	//while vertex has connections
	for (int i = 0; temp != NULL; temp = temp->next) {
		i = temp->end;
		if (!visited[i])//if unvisited, recurse function with new vertex [i]
			connectivity(g, i, visited);
	}
}

//function to show if graph is connected
int isConnected(Graph* g)
{
	//visited -> array to hold 0 if vertex isn't visited, 1 - if visited. size -> amount of vertices
	int visited[MAX]; int size = g->verticesAmount;
	for (int i = 0; i < MAX; ++i) visited[i] = 0; //all vertices are unvisited yet

	int startVertex = 0; //a vertex with the smallest number

	//finding the smallest vertex which aren't NULL
	for (int i = 0; i < MAX; i++)
		if (g->edges[i] != NULL) {
			startVertex = i;
			break;
		}

	//check for connectivity
	connectivity(g, startVertex, visited);

	int visitedCounter = 0; //variable to count how many vertices are visited

	for (int i = 0; i < MAX; i++)
		if (visited[i]) //if vertex is visited - increase counter
			visitedCounter++;

	if (visitedCounter != size) // if amount of visited vertices aren't equal to their true amount - disconnected
		return 0;

	return 1; // graph is connected

}

//function to choose the variant of graph's initialization
int inputGraph(Graph* g)
{
	//modeNumber for choosing the input mode, MODE - validation control variable
	int modeNumber, mode = 0;

	printf("Hello, I'm Hamilton Cycle Solution, please lead all the instructions :) ");

	printf("\nIf you want to read from keyboard - press 1, from file - press 2: ");
	do {
		
		if ((mode = scanf_s("%d", &modeNumber)) == 0 || (modeNumber != 1 && modeNumber != 2)) {
			printf("\nPlease enter 1 or 2, nothing else!\nTry again: ");  mode = 0;  getchar();
		}

	} while (!mode);//while modeNumber is not 1 or 2 input repeats

	//choosing input variant on base of modeNumber's value
	switch (modeNumber) {
	case 1: //keyboard input
		readGraph(g); return 1; break; 
	case 2:
		if (!readGraphFromFile(g)) //file input
			return 0;
		break;
	default:
		return 0;
	}

	return 1;
}

//funtion to initialize the graph from the file, returns 1 if successful, otherwise - 0
int readGraphFromFile(Graph* g)
{
		printf("\nIs your graph directed or not? 1 - yes, 0 - no: ");

		int isDirected; //variable that shows whether graph is directed, 1 - yes, 0 - no

		int mode = 0; //variable for input validation

		do {

			if ((mode = scanf_s("%d", &isDirected)) == 0 || (isDirected != 0 && isDirected != 1)) {
				printf("\nPlease enter 1 or 0, nothing else!\nTry again: ");  mode = 0; getchar();
			}

		} while (!mode); mode = 0; //input repeats while user doesn't input 1 for directed or 0 for undirected graph

		initializeGraph(g, isDirected); //initializing empty graph

		int buffer[3]; //array to hold vertices of one Edge of the graph from file and weight between them
		int inputAttempts = 4; //variable to control attempts of inputing file's name

		FILE* file;
		char fileName[100]; //array to hold file's name

		printf("\nPlease enter the file name and way to it(if needed): ");
		getchar();
		do {
			gets_s(fileName, 100); inputAttempts--; //after input, num of attempts decreases
			fopen_s(&file, fileName, "r"); if (!inputAttempts) break;

			//if file's name is wrong or another problems caused situation that file isn't opened
			if (file == NULL) {
				printf("\nProblems with opening file! Please check whether the name and the way is valid.\nTry again(%d attempts left):", inputAttempts - 1);
			}
			else mode = 1; //if everything's okay, break the cycle

		} while (!mode);

		//if were used all the attempts - input is over, finish working with the program
		if (!inputAttempts) {
			printf("\nAttempts are over!\n");
			system("pause");
			return 0;
		}

		//reading from file edge's vertices and weigt
		while (1) {

			//reading... if there's no 3 numbers or we reached the end of file
			if (fscanf_s(file, "%d %d %d", &buffer[0], &buffer[1], &buffer[2]) != 3 && !feof(file)) {
				printf("\nThere are some invalid data in file, correct it!"); break;
			}
			else if (feof(file)) { //counting vertices amount if input is over
				for (int i = 0; i < MAX; i++) {
					if (g->edges[i] != NULL)
						g->verticesAmount++;
				}
				break;
			}
			else //if reading was OK, initializing the edge with nums
				insertEdge(g, buffer[0], buffer[1], buffer[2], isDirected);
		}

		if (isConnected(g)) //checking if graph is connected
			return 1; //indicator of completed input
		else {
			printf("\nThe graph you have in file is not connected! Please fix it and retry.\n"); 
			return 0; //indicator of fail
		}
}

//initializing an empty graph
void initializeGraph(Graph *g, int isDirected)
{
	int counter; //variable to go through the graph

	g->verticesAmount = 0;
	g->edgesAmount = 0;
	g->isDirected = isDirected;
	for (counter = 0; counter < MAX; counter++) g->edges[counter] = NULL;
}

//initializing the edge of the graph
void insertEdge(Graph *g, int x, int y, int weight, int isDirected)
{
	EdgeNode* temp;//temporary pointer
	temp = (EdgeNode*)malloc(sizeof(EdgeNode));//allocation memory for edge

	if (temp == NULL) {
		printf("\nProblems with memory allocation!");
		return;
	}

	temp->weight = weight;
	temp->end = y;
	temp->next = g->edges[x];

	g->edges[x] = temp;

	//if undirected - add antagonic [Y-X] edge
	if (!isDirected) {
		insertEdge(g, y, x, weight, 1);
	}
	else { //counting edges amount
		g->edgesAmount++;
	}
}

//inputing the graph from keyboard
void readGraph(Graph *g)
{
	int graphConnectivity = 0;//variable to check graph's connectivity
	do {
		printf("\nIs your graph directed or not? 1 - yes, 0 - no: ");

		int isDirected;

		int mode = 0; //variable for input validation

		do {

			if ((mode = scanf_s("%d", &isDirected)) == 0 || (isDirected != 0 && isDirected != 1)) {
				printf("\nPlease enter 1 or 0, nothing else!\nTry again: ");  mode = 0; getchar();
			}

		} while (!mode); mode = 0;//cycle repeats while user doesn't input 1 for directed, 0 - for undirected graph

		int x, y, weight;//vertices in edge(x,y)

		initializeGraph(g, isDirected);//initializing an empty graph

		printf("\nPlease enter: start point, then finish point and weight between(if you want to stop just enter -1 on any):\n");

		while (1) {
			do {

				if ((mode = scanf_s("%d %d %d", &x, &y, &weight)) == 0 || x < -1 || y < -1 || weight < -1) {
					printf("\nIncorrect input! Data should be bigger or equal to 0, -1 on any to exit.\nTry again(start point, then finish point and weight between): ");  mode = 0; getchar();
				}

			} while (!mode); mode = 0; //repeats while user doesn't input correct data or exit

			if (x != -1 && y != -1 && weight != -1) //if everything's OK - initialize the edge
				insertEdge(g, x, y, weight, isDirected);
			else { //exit - counting the vertices
				for (int i = 0; i < MAX; i++) {
					if (g->edges[i] != NULL)
						g->verticesAmount++;
				}
				break; //breaking the cycle
			}
		}

		if (isConnected(g)) //checking if graph is connected
			graphConnectivity = 1; //indicator of completed input
		else {
			printf("\nThe graph you entered is not connected! Please fix it and retry.\n");
		}

	} while (!graphConnectivity);
}


//printing the graph out
void printGraph(Graph* g)
{
	int counter; //variable to go through the graph
	EdgeNode* temp; //temporary pointer

	for (counter = 0; counter < MAX; counter++) {
		temp = g->edges[counter]; //temp points to vertex that equals to counter

		while (temp != NULL) { // while there's any vertices this vertex is connected with
			printf("\n(%d->%d[weight:%d])", counter, temp->end, temp->weight);
			temp = temp->next; //moving on to the next vertex
		}
	}
	printf("\n");
}

//deleting memory allocated for the graph
void freeGraph(Graph* g)
{
	for (int i = 0; i < MAX; i++) {
		if (g->edges[i] != NULL) //if vertex [i] exists
			free(g->edges[i]);
	}

	free(g);
}

//function to checking if this Hamilton Path is Cycle or just a path
int isCycle(Graph* g, int currentVertex, int startVertex)
{
	EdgeNode* temp = g->edges[currentVertex];//points to the last vertex

	int isCycle = 0; //variable to show if it is cycle[1 - yes, 0 - no]

	while (temp != NULL) { //while there's any vertex connected

		if (temp->end == startVertex) { //if this vertex is connected with start vertex - that's cycle
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
	EdgeNode* temp = g->edges[startVertex]; //temporary pointer

	//while has connected vertices
	while (temp != NULL) {
		if (temp->end == endVertex) //if vertex is the same - break
			break;
		else temp = temp->next;
	}

	return temp->weight; 

}

//funtion to print Hamilton Cycle
void printCycle(Graph* g, int* fullPath, int size)
{
	printf("\nHamilton Cycle(x [weight] y): ");

	for (int i = 0, j = 0; i < size; i++) {
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
	static unsigned counter = 1, wght = 0; //counter of visited vertices, added weights

	//base for recurtion
	if (counter == size) { //if all the vertices are visited
		if (isCycle(g, vertex, path[0])) { //check if this is cycle
			printCycle(g, path, size); *existCycle = 1;//cycle found
			return;
		}

		else return;
	}

	EdgeNode* temp; // temporary pointer
	temp = g->edges[vertex]; //points to the current vertex to see then all the vertices connected with it

	//while there's any connected vertex yet and cycle is unfound
	for (int i = 0; temp != NULL && !(*existCycle); temp = temp->next) {
		i = temp->end; //i = vertex connected with current

		if (!visited[i]) { //if vertex unvisited
			visited[i] = 1; //mark it visited
			path[counter++] = i; //increase the path adding this vertex

			HamiltonCycle(g, path, visited, size, i, existCycle); //recurse the function with this unvisited early vertex [i]
			
			//backtracking
			visited[i] = 0; //if it didn't lead to cycle, deleting the last vertex from visited
			path[--counter] = -1; // and decreasing the path's counter
		}

	}

}


//function to check if there's such a vertex in graph
int doesVertexExist(Graph* g, int vertex)
{
	return (g->edges[vertex] != NULL) ? 1 : 0; //if pointer that points to vertex's num isn't NULL - exists
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


	int path[MAX];//array to hold Hamilton Path

	path[0] = vertex; //path starts from vertex we are looking the path for

	int size = g->verticesAmount; //size - vertices amount to go through

	int visited[MAX]; // array to hold 1 if vertex is visited 0 if unvisited

	//initializing the array with default 0
	for (int i = 0; i < MAX; i++) {
		visited[i] = 0;
	} visited[vertex] = 1; //currect vertex is visited

	int existCycle = 0; //variable to check if any cycle has been found

	//finding solution, if there's any, it will be printed, otherwise - not
	HamiltonCycle(g, path, visited, size, vertex, &existCycle);

	//if path is empty - no cycle
	if (!existCycle)
		printf("\nThere's no Hamilton Cycle for this vertex!\n");

}
