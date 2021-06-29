// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

static NodeValues newNodeValue(Graph g);

static double predListLength(ShortestPaths sps, int t);

static void recursiveBetween(int s, int t, ShortestPaths sps, double *result);

static void setArray(double *array, int length);

/*
The function below is responsbile for gathering the relevant variables
required to conduct the Wasserman and Faust formula. This process involves
applying dijkstra's algorithm to each vertex and cycling through distances
between each selected vertex and other nodes in the graph.
*/
NodeValues closenessCentrality(Graph g) {

    int numN = GraphNumVertices(g);

	NodeValues nvs;
    nvs.values = malloc(numN*sizeof(double));
    nvs.numNodes = numN;

    int i = 0;
    double N = numN;
    double result; 
    //The while loop below is to apply dijkstra's algorithm to each node
    while (i <numN) {
        ShortestPaths sps = dijkstra(g, i);
            int j = 0;  
            double n = 1;
            double sum = 0;
            //The while loop below is to cycle through the dist array and 
            //summate the distances between the src vertex and others
            while (j < numN) {
                if (sps.dist[j]>0) {
                    if (j!=i) {
                        sum = sum + sps.dist[j];
                        n++;
                    }
                }
                j++;
            }
            //Once all distances are summated for the vertex, they are applied 
            //through the formula below
            result = (n-1)*(n-1)/((N-1)*sum);
            if (sum == 0) {
                result = 0;
            }
            nvs.values[i] = result;
        i++;
        freeShortestPaths(sps);
    }            
	return nvs;
}

/*
The function below recreates the output gained from dijkstra's algorithm
 and loops through each predArray, giving the head predNode to the recursive
function above. The results of the recursive function are then transferred 
to the nvs.values array
*/
NodeValues betweennessCentrality(Graph g) {
    
    NodeValues nvs = newNodeValue(g);
    int numN = GraphNumVertices(g);
    double *result = malloc(numN * sizeof(double));
    setArray(result, numN);
    int s = 0;
    //The loop below applies dijkstra to each node in the graph
    while (s<numN) {
        ShortestPaths sps = dijkstra(g, s);
        int t = 0;
        //The loop below iterates through each linked list in the pred node array
        while (t<numN) {
            //Calls the recursive function which doesn't need its output assigned as it 
            //does it through the result list instead which is called later
            recursiveBetween(s, t, sps, result);
            t++;
        }
        s++;
        freeShortestPaths(sps);
    }

    //The results from the the recursive function are transferred into nvs.values
    int k=0;
    while (k < numN) {
        nvs.values[k] = result[k];
        k++;
    }
    free(result);
    
	return nvs;
}

/*
This function generates and initialises an nvs array
*/
static NodeValues newNodeValue(Graph g) {

    NodeValues new = {0};
    int numN = GraphNumVertices(g);
    new.numNodes = numN;
    new.values = malloc(numN * sizeof(double));

    int i = 0;
    while (i<numN) {
        new.values[i] = 0;
        i++;
    }
    
    return new;    
}

/*
This function is used to assess the length of the pred list
*/
static double predListLength(ShortestPaths sps, int t) {
    int length = 0;
    PredNode *curr = sps.pred[t];
    while (curr!=NULL) {
        length++;
        curr = curr->next;
    }
    return length;
}

/*
This function takes the pointer to the array result and makes continuous changes. It also takes t and sps to
find the head of each pred list in the pred array. It initially finds a predNode then follows it through the pred
array, and adds to the results array as it goes through. The length function is also called for prednodes which 
are part of a list grater than 1 node.
*/
static void recursiveBetween(int s, int t, ShortestPaths sps, double *result){

    PredNode *curr = sps.pred[t];

    double length = predListLength(sps, t); //This finds the length of the predlist (sps.pred[t])
    
    //This loop iterates through the pred list and everytime a node is found, it is followed through the
    //Pred array until it reaches NULL
    while (curr != NULL) {
        if ((curr->v != t) && (curr->v !=s)) {
            double r = 1/length; 
            result[curr->v]+= r;    //adds into the result array to change a vertex's centrality when it is found
            recursiveBetween(s, curr->v, sps, result);
        }
        curr = curr->next;
    }
}

//This function initilises all indexes in the array back to 0
static void setArray(double *array, int length) {
    int i = 0;
    while (i < length) {
        array[i] = 0;
        i++;
    }
}


/*
The function below is used to apply the normalisation process to the
betweenness centrality function's results. By taking in the Graph g
as input, it calculates the number of nodes and applies that to the
provided formula
*/
NodeValues betweennessCentralityNormalised(Graph g) {

    NodeValues nvs = betweennessCentrality(g);

    int n = GraphNumVertices(g);
    //The while loop iterates through the results of betweenness
    //And multiplies each by 1/[(n-1)*n-2)]
    int i = 0;
    while (i < n) {
        nvs.values[i] = nvs.values[i]/((n-1)*(n-2));
        i++;
    }
    	
	return nvs;
}

/*
The function below iterates through the nvs.values array to print out 
the results from the betweenness and betweennes normalised functions
*/
void showNodeValues(NodeValues nvs) {
    int i=0;
    while (i < nvs.numNodes) {
        printf("%d: %f\n", i, nvs.values[i]);
        i++;
    }
}

/*
This function frees the values array in the NodeValues struct
*/
void freeNodeValues(NodeValues nvs) {

    assert(nvs.values != NULL);
    free(nvs.values);

}

