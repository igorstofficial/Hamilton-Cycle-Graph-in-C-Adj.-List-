#include <stdio.h>
#include <stdlib.h>
#include "graphFuncs.h"

void main(void)
{
	//creating graph
	Graph* graph;

	//checking if memory was allocated well
	if ((graph = (Graph*)malloc(sizeof(Graph))) == NULL) {
		printf("\nProblems with memory allocation!");
		return;
	}
	
	//choosing an input mode and initializing the graph
	if (!inputGraph(graph)) //if there were problems with file
		return;

	//printing the graph out
	printGraph(graph);

	//finding and printing the Hamilton Cycle if it exists
	findHamiltonCycle(graph);

	//deleting memory allocated for graph
	freeGraph(graph);
	
	//pausing for checking results
	system("pause");
}
