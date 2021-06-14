# COMP2521: Assignment 2
# Social Network Analysis

## Objectives
- to implement graph based data analysis functions (ADTs) to mine a given social network.
- to give you further practice with C and data structures (Graph ADT)

## Aim
In this assignment, your task is to implement graph based data analysis functions (ADTs) to mine a given social network. For example, detect say "influenciers", "followers", "communities", etc. in a given social network. You should start by reading the Wikipedia entries on these topics. Later I will also discuss these topics in the lecture.

[Social network analysis](https://en.wikipedia.org/wiki/Social_network_analysis)
[Centrality](https://en.wikipedia.org/wiki/Centrality)
The main focus of this assignment is to calculate measures that could identify say "influenciers", "followers", etc., and also discover possible "communities" in a given social network.

## Dos and Don'ts !
Please note that,

For this assignmet you can use source code that is available as part of the course material (lectures, exercises, tutes and labs). However, you must properly acknowledge it in your solution.
All the required code for each part must be in the respective `*.c file`.
You may implement additional helper functions in your files, please declare them as "static" functions.
After implementing Dijkstra.h, you can use this ADT for other tasks in the assignment. However, please note that for our testing, we will use/supply our implementation of Dijkstra.h. So your programs MUST NOT use any implementation related information that is not available in the respective header files (`*.h files`). In other words, you can only use information available in the corresponding `*.h files`.
Your program must not have any "main" function in any of the submitted files.
Do not submit any other files. For example, you do not need to submit your modified test files or `*.h files`.
If you have not implemented any part, must still submit an empty file with the corresponding file name.


## Provided Files
We are providing implementations of Graph.h and PQ.h . You can use them to implement all three parts. However, your programs MUST NOT use any implementation related information that is not available in the respective header files (*.h files). In other words, you can only use information available in the corresponding *.h files.

Also note:

- all edge weights will be greater than zero.
- we will not be testing reflexive and/or self-loop edges.
- we will not be testing the case where the same edge is inserted twice.
Download files:

Ass2_files.zip
Ass2_Testing.zip

## Part-1: Dijkstra's algorithm
In order to discover say "influencers", we need to repeatedly find shortest paths between all pairs of nodes. In this section, you need to implement Dijkstra's algorithm to discover shortest paths from a given source to all other nodes in the graph. The function offers one important additional feature, the function keeps track of multiple predecessors for a node on shortest paths from the source, if they exist. In the following example, while discovering shortest paths from source node '0', we discovered that there are two possible shortests paths from node '0' to node '1' (0->1 OR 0->2->1), so node '1' has two possible predecessors (node '0' or node '2') on possible shortest paths, as shown below.

We will discuss this point in detail in a lecture. The basic idea is, the array of lists ("`pred`") keeps one linked list per node, and stores multiple predecessors (if they exist) for that node on shortest paths from a given source. In other words, for a given source, each linked list in "pred" offers possible predecessors for the corresponding node.

	
                  Node 0
                    Distance
                      0 : X
                      1 : 2
                      2 : 1
                    Preds
                      0 : NULL
                      1 : [0]->[2]->NULL 
                      2 : [0]->NULL
                  
                  Node 1
                    Distance
                      0 : 2
                      1 : X
                      2 : 3
                    Preds
                      0 : [1]->NULL
                      1 : NULL
                      2 : [0]->NULL
                  
                  Node 2
                    Distance
                      0 : 3
                      1 : 1
                      2 : X
                    Preds
                      0 : [1]->NULL
                      1 : [2]->NULL
                      2 : NULL
               
The function returns 'ShortestPaths' structure with the required information (i.e. 'distance' array, 'predecessor' arrays, source and no_of_nodes in the graph)

Your task: In this section, you need to implement the following file:

Dijkstra.c that implements all the functions defined in Dijkstra.h.

## Part-2: Centrality Measures for Social Network Analysis
Centrality measures play very important role in analysing a social network. For example, nodes with higher "betweenness" measure often correspond to "influencers" in the given social network. In this part you will implement two well known centrality measures for a given directed weighted graph.

Descriptions of some of the following items are from Wikipedia at Centrality, adapted for this assignment.

### Closeness Centrality
Closeness centrality (or closeness) of a node is calculated as the sum of the length of the shortest paths between the node (x) and all
other nodes (y ∈ V ∧ y ≠ x) in the graph. Generally closeness is defined as below,

C (x)=1/∑yd(y,x)

where 
d(y,x) is the shortest distance between vertices x and y







