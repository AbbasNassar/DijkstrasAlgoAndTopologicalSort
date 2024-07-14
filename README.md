# Campus Building and Course Management System

## Overview
This project involves implementing a system to manage buildings and courses within a campus using graph and queue data structures. The main functionalities include loading data, calculating the shortest distance between buildings, printing the shortest route, and sorting courses using topological sort.

## Structure
The project consists of the following main components:
- Graph structure to manage buildings and their distances.
- Queue structure to manage courses and their dependencies.
- Functions to load data, calculate shortest paths, and perform topological sorting.

## Graph Structure
```c
#define maxNumOfBuildings  9
#define maxNumOfCourses 11
#define Inf INT_MAX

// Graph structure
struct graph;
typedef struct graph* graphPtr;

// Heap structure using queues
struct queueNode;
typedef struct queueNode* queueNodePtr;

struct queue;
typedef struct queue *queuePtr;

struct graphCourses;
typedef struct graphCourses* coursesGraphPtr;

struct destBuilding;
typedef struct destBuilding *ptrToDestBuilding;
typedef struct destBuilding *destBuildingsList;

struct destBuilding {
    char buildingName[32];
    unsigned int distance;
    bool visited;
    ptrToDestBuilding next;
};

struct graph {
    char destBuildings[maxNumOfBuildings][20];
    destBuildingsList buildingsGraph[maxNumOfBuildings];
};

struct queue {
    queueNodePtr front;
    queueNodePtr rear;
};

struct queueNode {
    char courseName[20];
    queueNodePtr next;
    queueNodePtr prev;
};

struct graphCourses {
    char Courses[maxNumOfCourses][20];
    queueNodePtr CoursesGraph[maxNumOfCourses];
};
```

## Functions
### Graph and Queue Operations
* Queue Operations
```c
queuePtr createCoursesQueue();
queueNodePtr createCourseNode(char word[]);
void enqueue(queuePtr q, char word[]);
void dequeue(queuePtr q);
```
* Graph Operations
```c
coursesGraphPtr createCoursesGraph();
void loadCourses(coursesGraphPtr graph);
void insertCourseList(queueNodePtr L, char Course[]);
void insertCourseGraph(coursesGraphPtr graph, queueNodePtr list);
void printCourseGraph(coursesGraphPtr graph);
graphPtr createGraph();
destBuildingsList createDestBuildingsList();
int getIndex(graphPtr graph, char token[]);
void loadBuildingsAndDistances(graphPtr graph);
void insertBuildings(graphPtr graph, char insertedSrcBuilding[], char insertedDestBuilding[], int insertedDistance);
```

## Algorithms
* Shortest Path Calculation (Dijkstra's Algorithm)
```c
void printPath(int parent[], int j, graphPtr graph) {// Recursive function to print the path, because the path is not linear.
    if (parent[j] == -1) {
        return;
    }
    printPath(parent, parent[j], graph);
    printf(" -> %s", graph->destBuildings[j]);
}
void printResult(unsigned int dist[], int parent[], graphPtr graph, int src, int dest, bool printPathFlag) {// Used to print the shortest distance between two nodes
    printf("Shortest Path from %s to %s is with Distance: %d\n", graph->destBuildings[src], graph->destBuildings[dest], dist[dest]);
    if (printPathFlag){
        printf("Path: %s", graph->destBuildings[src]);
        printPath(parent, dest, graph);
    }
    printf("\n");
}
int minDistance(unsigned int distance[],unsigned int sptSet[]){//find vertex with less distance
    unsigned int min = Inf;
    int min_index;

    for (int v = 0; v < maxNumOfBuildings; v++){
        if (sptSet[v] == 0 && distance[v] <= min){
            min = distance[v];
            min_index = v;
        }
    }

    return min_index;
}
void dijkstra(graphPtr graph, char source[], char destination[], bool printPath) {
    // This function didn't use Heap, unfortunately.
    // Used contingency list to implement the graph.
    unsigned int distance[maxNumOfBuildings];// Used to keep the minimum distances from the source vertex to other vertices.
    unsigned int shortestPathReached[maxNumOfBuildings];// This array indicates that the shortest path from the source vertex to the i-th vertex is reached.
    int parent[maxNumOfBuildings];// This array hold the nearest father of the i-th vertex

    for (int i = 0; i < maxNumOfBuildings; i++) {// Initialize arrays
        parent[i] = -1;
        distance[i] = Inf;
        shortestPathReached[i] = 0;
    }

    int srcBuildingIndex = getIndex(graph, source);// Make sure inserted building are in the graph
    int destBuildingIndex = getIndex(graph, destination);

    if (srcBuildingIndex == -1) {
        printf("Source building not found\n");
        return;
    }
    if (destBuildingIndex == -1) {
        printf("Destination building not found\n");
        return;
    }

    distance[srcBuildingIndex] = 0;// The distance to the source vertex is zero

    for (int count = 0; count < maxNumOfBuildings - 1; count++) {
        int u = minDistance(distance, shortestPathReached);// Update the shortest distances for vertices from the source
        if (u == -1) {
            break; // All reachable vertices processed
        }
        shortestPathReached[u] = 1;

        if (u == destBuildingIndex) {
            break;
        }

        ptrToDestBuilding temp = graph->buildingsGraph[u];
        while (temp != NULL) {
            int v = getIndex(graph,temp->buildingName);
            if (!shortestPathReached[v] && distance[u] != Inf && distance[u] + temp->distance < distance[v]) {// Update parent and distance for vertex in index v.
                parent[v] = u;
                distance[v] = distance[u] + temp->distance;
            }
            temp = temp->next;
        }
    }

    printResult(distance, parent, graph, srcBuildingIndex, destBuildingIndex,printPath);
}
```
* Topological Sort
```c
void topologicalSort( coursesGraphPtr graph ) {
    char topRes[maxNumOfCourses][20];// Will hold the result of topological sort.
    int inDegree[maxNumOfCourses];// Will hold the values of inDegrees for each vertex.
    for (int i = 0; i < maxNumOfCourses; i++) {// Initialize inDegree array.
        inDegree[i] = 0;
    }
    for (int i = 0; i < maxNumOfCourses; i++) {// Update the values of inDegrees for all vertices.
        char course[20];
        strcpy(course, graph->Courses[i]);
        for (int j = 0; j < maxNumOfCourses; j++) {
            if (i == j) {
                continue;
            } else {
                queueNodePtr temp = graph->CoursesGraph[j];
                while (temp != NULL) {
                    if (strcmp(course, temp->courseName) == 0) {
                        inDegree[i]++;
                    }
                    temp = temp->next;
                }
            }
        }
    }
    queuePtr queue = createCoursesQueue();
    for (int i = 0; i < maxNumOfCourses; i++) {// Enqueue vertices with zero inDegrees Initially.
        if (inDegree[i] == 0) {
            enqueue(queue, graph->Courses[i]);
        }
    }
    int resIndex = 0;// To keep the update on result array IN-ORDER.
    while (queue->front != NULL) {// All elements dequeued from the queue is with inDegree zero.
        dequeue(queue);
        strcpy(topRes[resIndex], tokenCommunicate);// Store in result array (IN-ORDER).
        resIndex++;
        for (int i = 0; i < maxNumOfCourses; i++) {// Get neighbouring vertices and decrement their inDegree values.
            if (strcmp(graph->Courses[i], tokenCommunicate) == 0) {
                queueNodePtr current = graph->CoursesGraph[i];
                while (current != NULL) {
                    for (int j = 0; j < maxNumOfCourses; j++) {
                        if (strcmp(graph->Courses[j], current->courseName) == 0) {
                            inDegree[j]--;
                            if (inDegree[j] == 0) {// If one of the neighbouring vertices had an inDegree value of 0, enqueue to the queue.
                                enqueue(queue, graph->Courses[j]);
                            }
                            break;// Break inner loop and update other neighbouring vertices
                        }
                    }
                    current = current->next;
                }
                break;
            }

        }
    }
    if (resIndex == maxNumOfCourses){// This means that one or more vertices can't reach 0-InDegree which indicates the presence of a cycle :().
        printf("Topological Order:\n");
        for (int i = 0; i < resIndex; i++) {
            printf("%s\n", topRes[i]);
        }
    }
    else
        printf("The loaded courses graph contains a cycle:(\n");
}
```
# Main Function
```c
int main() {
    coursesGraphPtr coursesGraph = createCoursesGraph();
    graphPtr graph = createGraph();

    int isDataLoaded = 0;

    char srcBuilding[20];
    char desBuilding[20];
    bool printPath = 0; // 0: Don't print, 1: Print
    int selection = 0;

    do {
        printf("1- Load data from the files.\n");
        printf("2- Calculate the shortest distance between two buildings.\n");
        printf("3- Print the shortest route between two buildings and the total distance.\n");
        printf("4- Sort the courses using the topological sort and print result.\n");
        printf("5- Exit the application.\n");
        scanf("%d", &selection);

        if (selection == 1) {
            loadBuildingsAndDistances(graph);
            isDataLoaded = 1;
            printf("Data is loaded successfully:)\n");
        } else if (selection == 2) {
            if (!isDataLoaded) {
                printf("You must load data from the files first!!\n");
                continue;
            }
            printf("Source building:\n-");
            scanf("%s", srcBuilding);
            printf("Destination building:\n-");
            scanf("%s", desBuilding);
            dijkstra(graph, srcBuilding, desBuilding, printPath);
        } else if (selection == 3) {
            if (!isDataLoaded) {
                printf("You must load data from the files first!!\n");
                continue;
            }
            printf("Source building:\n-");
            scanf("%s", srcBuilding);
            printf("Destination building:\n-");
            scanf("%s", desBuilding);
            printPath = 1;
            dijkstra(graph, srcBuilding, desBuilding, printPath);
        } else if (selection == 4) {
            loadCourses(coursesGraph);
            topologicalSort(coursesGraph);
        } else if (selection == 5) {
            free(graph);
            free(coursesGraph);
            exit(0);
        } else {
            printf("Invalid Selection:(\n");
        }
    } while (1);
}
```

## Usage
1- Load Data from Files: Load building distances and course dependencies.
2- Calculate Shortest Distance: Find the shortest distance between two buildings using Dijkstra's algorithm.
3- Print Shortest Route: Print the shortest route between two buildings along with the total distance.
4- Topological Sort of Courses: Sort and print courses using topological sort to resolve dependencies.

## Notes
* Ensure the data files `../Courses.txt` and `../buildings.txt` are in place and formatted correctly.
* The maximum number of buildings and courses are defined by `maxNumOfBuildings` and `maxNumOfCourses` respectively. Adjust these constants as needed.
