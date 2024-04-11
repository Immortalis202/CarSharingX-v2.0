#pragma once
#include "stdbool.h"

#define MAX_CITY 300
#define MAX_CAR 100

typedef int Weight;
typedef char Label[32];

//* struct to represent the car in a node
typedef struct Car{
    Label label;
    long autonomy;
}Car ;

//* Structure to represent a node in the graph
typedef struct Node {
    Label label;
    Car cars[MAX_CAR];
    int id; 
    int numCar;           // Unique identifier for the node
    int heuristic;     // Heuristic value for A* algorithm
    double latitude;
    double longitude;
    struct Edge *edges;
    int modified; // Adjacency list of edges
} Node;

extern Node graph[MAX_CITY];
extern int cityAdded;

//* Structure to represent an edge in the graph
typedef struct Edge {
    int dest;    // Destination node's identifier
    Weight weight;  // Weight of the edge
    struct Edge *next; // Pointer to the next edge in the list
} Edge;

//* Structure to represent a priority queue node for A* algorithm
typedef struct PQNode {
    int id;        // Identifier of the node
    int f;         // f = g + h (total cost)
    int g;         // Cost from the start node
    struct PQNode *next; // Pointer to the next node in the priority queue
} PQNode;

//* Function to create a new node
extern Node *createNode(Label name,int id, double lat, double lon, double heuristic);

//* Function to create a new edge
extern Edge *createEdge(int dest, Weight weight);

//* Function to add an edge to the graph
extern void addEdge(Node *graph, int src, int dest, Weight weight);

//* Function to add a car to a city
extern void addCar(Node *node, Label carName, long autonomy);

//* Function to create a new priority queue node
extern PQNode *createPQNode(int id, int f, int g);

//* Function to push a node into the priority queue
void push(PQNode **front, int id, int f, int g);

//* Function to pop a node from the priority queue
int pop(PQNode **front);

//* Function to modify the heuristic value of a node using vincenty distance to the goal
void modifyHeuristic(Node *graph, int nodeId, double goalLat, double goalLon);

//* Function to implement A* algorithm
extern void astar(Node *graph, int start, int goal, int numNodes);

//* Function to search for highest autonomy car
double findMaxAutonomy(Node node);

//* Function to check if an edge already exists
bool edgeExists(Node graph[], int src, int dest);

//* Function to check if the distance between two City is higher than the max autonomy car
bool checkDistance(double distance, Node node);

//* Function to traverse all the node and create node between them
void createAllEdge(int numNodes, Node graph[]);

//* Function to search for a specific city
int findCity(Node graph[], int numNodes,Label label);

//* Function to search for a specific car
void findCar(Node graph[],int id);

//* Function to remove a city and all the the relative edge and car
void removeCity(Node graph[], int id);

//* Function to remove a car from a city
void removeCar(Node graph[], int id, Label label);

//* Function to print all the city name
void allCity(Node graph[]);

//* Function to print the information of a city
void printCity(int id);

//* Function to print the information of all car in a city
void printAllCar(int id);

//* Function to print the information of a car
void printCar(int id, int idCar);

void populateGraph();