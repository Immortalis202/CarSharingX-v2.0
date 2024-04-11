#include "include/graphUtility.h"
#include "include/commons.h"
#include "include/server.h"
// * DEFINITION OF GRAPH
Node graph[MAX_CITY];

char terminator[11] ="gabbopower";

// Function to create a new node
Node *createNode(Label name,int id, double lat, double lon, double heuristic) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if(newNode == NULL){
        printf("Failed to create new node \n");
    }
    newNode->id = id;
    newNode->numCar = 0;
    newNode->heuristic = heuristic;
    newNode->longitude = lon;
    newNode->latitude = lat;
    strcpy(newNode->label,name);
    newNode->edges = NULL;
    cityAdded++;

    return newNode;
}

// Function to create a new edge
Edge *createEdge(int dest, Weight weight) {
    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = NULL;
    //printf("Added edge to %d \n", dest);
    return newEdge;
}

// Function to add an edge to the graph
void addEdge(Node *graph, int src, int dest, Weight weight) {
    Edge *newEdge = createEdge(dest, weight);
    newEdge->next = graph[src].edges;
    graph[src].edges = newEdge;
}
//TODO change arguments from node to id
void addCar(Node *node, Label carName, long autonomy){
   // printf("si rompe qui \n");

    Car *newCar = (Car*) malloc(sizeof(Car));
    if(newCar == NULL){
        printf("Failed to create new car \n");
    }
   // printf("si rompe qui1 \n");
    
    newCar->autonomy = autonomy;
   // printf("si rompe qui2 \n");

    strcpy(newCar->label, carName);
   // printf("si rompe qui3 \n");

    node->cars[node->numCar] = *newCar;
    node->numCar++;
   // printf("si rompe qui4 \n");

    printf("Added new car %s \n",newCar->label);

}

// Function to create a new priority queue node
PQNode *createPQNode(int id, int f, int g) {
    PQNode *newNode = (PQNode *)malloc(sizeof(PQNode));
    newNode->id = id;
    newNode->f = f;
    newNode->g = g;
    newNode->next = NULL;
    return newNode;
}

// Function to push a node into the priority queue
void push(PQNode **front, int id, int f, int g) {
    PQNode *newNode = createPQNode(id, f, g);
    if (*front == NULL || (*front)->f >= f) {
        newNode->next = *front;
        *front = newNode;
    } else {
        PQNode *temp = *front;
        while (temp->next != NULL && temp->next->f < f) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

// Function to pop a node from the priority queue
int pop(PQNode **front) {
    if (*front == NULL) {
        return -1;
    } else {
        PQNode *temp = *front;
        int id = temp->id;
        *front = (*front)->next;
        free(temp);
        return id;
    }
}


// Function to modify the heuristic value of a node using vincenty distance to the goal
void modifyHeuristic(Node *graph, int nodeId, double goalLat, double goalLon) {
    // Assuming each node has latitude and longitude information
    double nodeLat = graph[nodeId].latitude;
    double nodeLon = graph[nodeId].longitude;

    // Calculate the Haversine distance between the node and the goal
    double distance = vincenty_distance(nodeLat, nodeLon, goalLat, goalLon);

    // Assign the Haversine distance as the new heuristic value for the node
    graph[nodeId].heuristic = distance;
}

// Function to implement A* algorithm
void astar(Node *graph, int start, int goal, int numNodes) {
    // Array to store the cost from start to each node
    double g[numNodes];
    // Array to store the parent node of each node
    int parent[numNodes];
    // Priority queue to store the nodes to be explored
    PQNode *front = NULL;

    // Initialize the g array with infinity and parent array with -1
    for (int i = 0; i < numNodes; i++) {
        g[i] = INT_MAX;
        parent[i] = -1;
    }

    // Set the cost of the start node to 0 and push it into the priority queue
    g[start] = 0;
    push(&front, start, graph[start].heuristic, 0);

    while (front != NULL) {
        // Pop the node with the minimum f value from the priority queue
        int current = pop(&front);

        // If the goal node is reached, break out of the loop
        if (current == goal) {
            break;
        }

        // Traverse through the adjacency list of the current node
        Edge *edge = graph[current].edges;
        while (edge != NULL) {
            // Calculate the tentative g value for the neighbor node
            int tentative_g = g[current] + edge->weight;

            // If the tentative g value is less than the current g value of the neighbor
            // update the g value and push the neighbor node into the priority queue
            if (tentative_g < g[edge->dest]) {
                g[edge->dest] = tentative_g;
                parent[edge->dest] = current;
                push(&front, edge->dest, g[edge->dest] + graph[edge->dest].heuristic, g[edge->dest]);
            }

            edge = edge->next;
        }
    }

    // Print the shortest path from start to goal
    printf("Shortest path from %d to %d: ", start, goal);
    int current = goal;
    while (current != -1) {
        printf("%d ", current);
        current = parent[current];
    }
    printf("\n");
}
//TODO must be called every time we add a new car
double findMaxAutonomy(Node node){
    double max = 0;
    for(int i = 0; i < node.numCar; i++){
        if(node.cars[i].autonomy > max){
            max = node.cars[i].autonomy;  
        }
    }
  //  printf("%lf \n",max);
    return max;
}

bool edgeExists(Node graph[], int src, int dest) {
    // Check if the source and destination nodes are valid
    if (src < 0 || src >= cityAdded || dest < 0 || dest >= cityAdded) {
        return false;
    }

    // Iterate through the adjacency list of the source node
    Edge* currentEdge = graph[src].edges;
    while (currentEdge != NULL) {
        // If the destination node is found in the adjacency list of the source node, return true
        if (currentEdge->dest == dest) {
            return true;
        }
        currentEdge = currentEdge->next;
    }

    // If the destination node is not found in the adjacency list of the source node, return false
    return false;
}

// TODO must be called every time we add a new car
bool checkDistance(double distance, Node node){

    double maxAutonomy = findMaxAutonomy(node);
        if(distance > maxAutonomy){
            return false;
        }else{
            return true;
        }
    }


void createAllEdge(int numNodes, Node graph[]){
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i != j) {
                double weight = vincenty_distance(graph[i].latitude, graph[i].longitude, graph[j].latitude, graph[j].longitude);
                //should check if edge already exist
                    if(edgeExists(graph,i,j) == false){
                        if(checkDistance(weight , graph[i] )== true) {
                            addEdge(graph, i, j, weight);
                        }
                    }else{
                        printf("Edge not created\n");
                    }
            }
        }
    }
}

int findCity(Node graph[], int numNodes,Label label){
    for(int i = 0; i < numNodes; i++){    
        if (strcasecmp(label, graph[i].label) == 0){
            return graph[i].id;
        }
    
    }
    return -1;
}

//* CLIENT CONVERSIONE DONE
void findCar(Node graph[],int id){
    char sendString[256];
    for(int i = 0; i < graph[id].numCar;i++){
        bzero(sendString, 256);
        sprintf(sendString,"Car %d: \n Name: %s \n Autonomy: %ld\n", i, graph[id].cars[i].label,graph[id].cars[i].autonomy );
        write(connfd, sendString, sizeof(sendString));
    }
    write(connfd,"gabbopower",11);
}

void removeCity(Node graph[], int id){
    printf("Removing city %s",graph[id].label);
    
    for(int i = 0; i< cityAdded; i++){
        Edge *prevEdge = NULL;
        Edge *currEdge = graph[i].edges;
        while(currEdge != NULL){
            if(currEdge->dest == id){
                if(prevEdge == NULL){
                    graph[i].edges = currEdge->next;
                }else {
                    prevEdge->next = currEdge->next;
                }
                Edge *temp = currEdge;
                currEdge = currEdge->next;
                free(temp);
            }else{
                prevEdge = currEdge;
                currEdge = currEdge->next;
            }
        }
        for(int i = id + 1; i < cityAdded; i++){
            graph[i-1] = graph[i];
        }
    }
    cityAdded--;
}

void removeCar(Node graph[], int id, Label label){
    int found = 0;
    for(int i = 0; i < graph[id].numCar; i++){
        if(strcmp(graph[id].cars[i].label,label) == 0){
            found = i;
        }
    }
    for(int i = found; i < graph[id].numCar-1; i++){
        graph[id].cars[i] = graph[id].cars[i+1];
    }
    graph[id].numCar--;

    printf("Removed car %s from %s",label, graph[id].label);
}

//! SERVER CONVERSION DONE
void allCity(Node graph[]){
    char sendString[256];
    sprintf(sendString,"List of all the city:\n");
    write(connfd, sendString, 256);
    for(int i=0; i < cityAdded;i++){
        bzero(sendString,255);
        sprintf(sendString,"%s \n",graph[i].label);
        write(connfd, sendString, 256);
    }
    write(connfd,terminator,11);
    return;
}
//! SERVER CONVERSION DONE
void printCity(int id){
    char sendString[256];
    sprintf(sendString,"City %s found \nName:      %s\nId:        %d\nCar Number:%d  \nLatitude:  %lf \nLongitude: %lf \n",
    graph[id].label,graph[id].label,graph[id].id,graph[id].numCar,graph[id].latitude,graph[id].longitude );            
    write(connfd, sendString, 256);        
}
//! SERVER CONVERSION DONE
void printAllCar(int id){
    char sendString[256];
    sprintf(sendString,"City: %s",graph[id].label);
    write(connfd, sendString, sizeof(sendString));
    for(int i = 0; i < graph[id].numCar; i++){
        printCar(id,i);
    }
}
//! SERVER CONVERSIONE DONE
void printCar(int id, int idCar){
        char sendString[256];
        sprintf(sendString,"Autonomy: %ld\n Name: %s ",graph[id].cars[idCar].autonomy,graph[id].cars[idCar].label); 
        write(connfd, sendString, sizeof(sendString));
}

void populateGraph() {
    graph[0] = *createNode("New York",cityAdded, 40.7128, -74.0060, 0); // New York
    graph[1] = *createNode("Los Angeles",cityAdded, 34.0522, -118.2437, 0); // Los Angeles
    graph[2] = *createNode("Chicago",cityAdded, 41.8781, -87.6298, 0); // Chicago
    graph[3] = *createNode("Houston",cityAdded, 29.7604, -95.3698, 0); // Houston
    graph[4] = *createNode("Phoenix",cityAdded, 33.4484, -112.0740, 0); // Phoenix

    addCar(&graph[0], "qualcosa", 11200);
    addCar(&graph[0], "quacosa", 300);
    addCar(&graph[0], "ciao1", 250);
    addCar(&graph[1], "qualcosa", 11200);
    addCar(&graph[1], "quacosa", 300);
    addCar(&graph[1], "ciao1", 250);
    addCar(&graph[2], "qualcosa", 11200);
    addCar(&graph[2], "quacosa", 300);
    addCar(&graph[2], "ciao1", 250);
    addCar(&graph[3], "qualcosa", 11200);
    addCar(&graph[3], "quacosa", 300);
    addCar(&graph[3], "ciao1", 250);
    addCar(&graph[4], "qualcosa", 1200);
    addCar(&graph[4], "quacosa", 300);
    addCar(&graph[4], "ciao1", 250);
    addCar(&graph[4], "ciao2", 250);
    addCar(&graph[4], "ciao3", 250);
    
    // Add edges to the graph
    createAllEdge(cityAdded, graph);
    // int found = 0;
    // found = findCity(graph,cityAdded, "Phoenix");
    // findCar(graph,4);

   // removeCar(graph, 4, "ciao2");

   // findCar(graph, 4);
  //  removeCity(graph, 3);

  //  allCity(graph);
    

    // Set the heuristic value of each node using Haversine distance to the goal node (e.g., Phoenix)
    double goalLat = graph[4].latitude;
    double goalLon = graph[4].longitude;
    for (int i = 0; i < MAX_CITY; i++) {
        modifyHeuristic(graph, i, goalLat, goalLon);
    }

    // Run A* algorithm from node 0 to node 4
    //astar(graph, 0, 4, MAX_CITY);
}




