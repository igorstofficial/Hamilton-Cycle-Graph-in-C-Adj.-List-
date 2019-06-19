#include "graphFuncs.h"
#include <stdio.h>
#include <stdlib.h>

//printing the graph out
void printGraph(Graph* g)
{
	int counter; //variable to go through the graph
	EdgeNode* temp; //temporary pointer

	for (counter = 0; counter < g->verticesAmount; counter++) {
		temp = g->edges[counter]; //temp points to vertex that equals to counter

		while (temp != NULL) { // while there's any vertices this vertex is connected with
			printf("\n(%d->%d[weight:%d])", temp->x, temp->y, temp->weight);
			temp = temp->next; //moving on to the next vertex
		}

	}
	printf("\n");
}

//deleting memory allocated for the graph
void freeGraph(Graph* g)
{
	EdgeNode* temp, *tempNext;

	for (int i = 0; i < g->verticesAmount; i++) {
		temp = g->edges[i];

		while (temp != NULL) {
			tempNext = temp->next;
			free(temp);
			temp = tempNext;
		}
	}

	free(g);
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

	int verticesAmount; // amount of vertices user thinks he will input

	printf("\nPlease enter how many vertices will be in graph in your opinion(don't have to be 100 percent accurate: ");

	do {

		if ((mode = scanf_s("%d", &verticesAmount)) == 0 || (verticesAmount <= 0)) {
			printf("\nPlease enter amount bigger than 0!\nTry again: ");  mode = 0; getchar();
		}

	} while (!mode); mode = 0; //input repeats while user doesn't input 1 for directed or 0 for undirected graph

	initializeGraph(g, isDirected, verticesAmount); //initializing empty graph

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
			for (int i = 0; i < g->size; i++) {
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
void initializeGraph(Graph *g, int isDirected, int size)
{
	int counter; //variable to go through the graph

	g->verticesAmount = 0;
	g->edges = (EdgeNode**)malloc(sizeof(EdgeNode*) * size);
	g->size = size;
	g->isDirected = isDirected;
	for (counter = 0; counter < size; counter++) g->edges[counter] = NULL;
}

//initializing the edge of the graph
void insertEdge(Graph *g, int x, int y, int weight, int isDirected)
{
	static int verticesAmount = 0;//counter to chech how many edges were added to control the memory reallocation 

	if (verticesAmount == g->size) { //reallocation in case of full graph
		EdgeNode** newEdges = (EdgeNode**)malloc(sizeof(EdgeNode*) * g->size * 2); // creating new List

		for (int i = 0; i < g->size; i++) //copying existing data
			newEdges[i] = g->edges[i];

		for (int i = g->size; i < g->size * 2; i++) //NULLing all new vertices
			newEdges[i] = NULL;

		free(g->edges); //deleting old list

		g->edges = newEdges; //copying new reallocated lsit
		g->size *= 2;//making size bigger in 2 times
	}

	//checking if there's already vertex [X]
	for (int i = 0; i < verticesAmount; i++) {

		if (g->edges[i]->x == x) {
			EdgeNode* tail = g->edges[i];

			while (tail->next != NULL) { //finding the last element in list
				tail = tail->next;
			}

			EdgeNode* preTail = tail;

			tail = tail->next;

			tail = (EdgeNode*)malloc(sizeof(EdgeNode)); //memory allocation

			if (tail == NULL) {
				printf("\nProblems with memory allocation!");
				return;
			}


			//initialization edge
			tail->x = x;
			tail->y = y;
			tail->weight = weight;
			tail->next = NULL;

			preTail->next = tail;

			//if undirected - add antagonic [Y-X] edge
			if (!isDirected) {
				insertEdge(g, y, x, weight, 1);
				return;
			}
			else return;
		}
	}

	EdgeNode* temp;//temporary pointer
	temp = (EdgeNode*)malloc(sizeof(EdgeNode));//allocation memory for edge

	if (temp == NULL) {
		printf("\nProblems with memory allocation!");
		return;
	}

	temp->x = x;
	temp->weight = weight;
	temp->y = y;
	temp->next = NULL;

	g->edges[verticesAmount++] = temp;

	//if undirected - add antagonic [Y-X] edge
	if (!isDirected) {
		insertEdge(g, y, x, weight, 1);
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

		int verticesAmount; // amount of vertices user thinks he will input

		printf("\nPlease enter how many edges will be in graph in your opinion(don't have to be 100 percent accurate: ");

		do {

			if ((mode = scanf_s("%d", &verticesAmount)) == 0 || (verticesAmount <= 0)) {
				printf("\nPlease enter amount bigger than 0!\nTry again: ");  mode = 0; getchar();
			}

		} while (!mode); mode = 0; //input repeats while user doesn't input 1 for directed or 0 for undirected graph


		initializeGraph(g, isDirected, verticesAmount);//initializing an empty graph

		printf("\nPlease enter: start point, then finish point and weight between(if you want to stop just enter -1 on any):\n");

		while (1) {
			do {

				if ((mode = scanf_s("%d %d %d", &x, &y, &weight)) == 0 || x < -1 || y < -1 || weight < -1) {
					printf("\nIncorrect input! Data should be bigger or equal to 0, -1 on any to exit.\nTry again(start point, then finish point and weight between): ");  mode = 0; getchar();
				}

			} while (!mode); mode = 0; //repeats while user doesn't input correct data or exit

			if (x != -1 && y != -1 && weight != -1) //if everything's OK - initialize the edge
				insertEdge(g, x, y, weight, isDirected);
			else { //exit - counting vertices
				for (int i = 0; i < g->size; i++) {
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