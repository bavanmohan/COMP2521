// Dijkstra ADT implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "PQ.h"

#define INFINITY 100000


static void insertPredNode(PredNode *node, int v, Graph g);

static PredNode *newNode (Vertex v, Graph g);

static ShortestPaths newPath(Graph g, Vertex src);

/*
This function below takes in Graph g and Vertex src, as the input an in turn
produces sps which outlines the distance from each verteddx to the src
*/
ShortestPaths dijkstra(Graph g, Vertex src) {
	assert(g != NULL);
	ShortestPaths sps = newPath(g, src);
    int numN = GraphNumVertices(g);	

    sps.numNodes = numN;
    sps.dist[src] = 0;
    
    //Initialises the PQ Queue, with source being the lowest prioirity
    PQ pq = PQNew();
    PQInsert(pq, src, 0);
    
    //The loop conducts a BFS by enqueing the vertex's neighbours and then
    //the neighbours' neighbours. Vertices with the smallest distance from src
    //are dequeued first
    while (PQIsEmpty(pq) == 0) {

        //The vertex with the smallest distance to src is dequeued first
        int v = PQDequeue(pq);

        //The vertex edge list is called and the loop below cycles through
        //all outgoing edges of the vertex
        AdjList edge = GraphOutIncident(g, v);
        while (edge != NULL) {
            //Below is the relaxation process where if a smaller distance to 
            //a node is found, it is replaced with the new smaller distance.
            //The PQ Queue and sps.dist array is updated to reflect the change
            if (sps.dist[v] + edge->weight < sps.dist[edge->v]) {
                sps.dist[edge->v] = sps.dist[v]+edge->weight;
                PQInsert(pq, edge->v, sps.dist[edge->v]);
                PredNode *temp = newNode(v,g);
                free(sps.pred[edge->v]);
                sps.pred[edge->v] = temp;
            }
            //The if statement below checks if the newfound distance is the same as
            //the smallest distance, and if so, calls the insertPredNode function
            //to skip to the end of the pred list and insert the new node.
            else if (sps.dist[v] + edge->weight == sps.dist[edge->v]) {
                sps.dist[edge->v] = sps.dist[v]+edge->weight;
                PQInsert(pq, edge->v, sps.dist[edge->v]);
                insertPredNode(sps.pred[edge->v], v, g);
            }
            edge = edge->next;
        }
    }
    PQFree(pq);

    //Vertices that remain unvisited will have their distance converted to 0;
    int count = 0;
    while (count < numN) {
        if (sps.dist[count] == INFINITY) {
            sps.dist[count] = 0;
        }
        count++;
    }
    
	return sps;
}

/*
This function takes the head of a specified pred list as well as vertex and g
to create a new node and insert the new node to the end of the pred list
*/
static void insertPredNode(PredNode *node, int v, Graph g){

    assert(node != NULL);
    PredNode *curr = newNode(v, g);
    PredNode *new = node;

    while(new->next != NULL) {
        new = new->next;
    }

    new->next = curr;
    curr->next = NULL;
}

//This function takes v and g to create a new PredNode
static PredNode *newNode (Vertex v, Graph g) {
    PredNode *new = malloc(sizeof(struct PredNode));
    
    new->v = v;
    new->next = NULL;
    
    return new;
}

/*
This function is used to initialise the ShortestPaths struct
Notable aspects include, a distance array, as well as the PredNode 
array which is an array of pointers to linkedlists
*/
static ShortestPaths newPath(Graph g, Vertex src) {

    int numN = GraphNumVertices(g);	

    ShortestPaths path;
    path.numNodes = 0;
    path.src = src;
    path.dist = malloc(numN*sizeof(int));

    PredNode **Array = malloc(numN*sizeof(PredNode*));
    path.pred = Array;
    //Initialise all predlist heads to NULL 
    int m = 0;
    while (m <numN) {
        path.pred[m] = NULL;
        m++;
    }
    //initialise all dist[] to infinity
    int i = 0;
    while (i<numN) {
        path.dist[i] = INFINITY;
        i++;
    }
	return path;
}


void showShortestPaths(ShortestPaths sps) {

}

/*
The function below is responsible for free the structures used above.
First, the distance array is freed. The the function iterates through the
pred array, starting at the head of each pred list, nodes for the individual 
list are freed then the eventually the list itself
*/
void freeShortestPaths(ShortestPaths sps) {
    
    assert(sps.dist != NULL);
    
    free(sps.dist);
    
    int i = 0;
    while (i < sps.numNodes) {
        PredNode *curr = sps.pred[i];
	    while (curr != NULL) {
            PredNode *temp = curr;
		    curr = curr->next;
		    free(temp);
	    }  
        i++;
    }
    free(sps.pred);
    
}
