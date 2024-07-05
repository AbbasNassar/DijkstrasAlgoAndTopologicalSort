/* Abbas Nassar 1210482 Sec.3 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
/* Didn't copy my style in commenting the previous projects due to time issues :(*/
#define maxNumOfBuildings  9
#define maxNumOfCourses 11
#define Inf INT_MAX
// This is used to instead of returning string from queue
char tokenCommunicate[20];
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

struct destBuilding
{
    char buildingName[32];
    unsigned int distance ;
    bool visited;
    ptrToDestBuilding next;
};
struct graph{
    char destBuildings[maxNumOfBuildings][20];
    destBuildingsList buildingsGraph[maxNumOfBuildings];
};
struct queue
{
    queueNodePtr front;
    queueNodePtr rear;
};
struct queueNode{
    char courseName[20];
    queueNodePtr next;
    queueNodePtr prev;
};
struct graphCourses{
    char Courses[maxNumOfCourses][20];
    queueNodePtr CoursesGraph[maxNumOfCourses];
};
queuePtr createCoursesQueue(); // Returns the head of the queue
queueNodePtr createCourseNode(char word[]);
void loadCourses(coursesGraphPtr graph);
void printCourseGraph(coursesGraphPtr graph);
void enqueue(queuePtr q, char word[]); // Insert always at the front of the queue
void dequeue(queuePtr q);              // Dequeue rear from queue and return its word
//void deleteQueue(queuePtr q);          // Delete the entire queue
//void printQueue(queuePtr q);

coursesGraphPtr createCoursesGraph();
void insertCourseList(queueNodePtr L, char Course[]);
queueNodePtr createCourseList();
void insertCourseGraph(coursesGraphPtr graph, queueNodePtr list);

destBuildingsList createDestBuildingsList();
int getIndex(graphPtr graph, char token []);// Create town List. Returns head of list.
graphPtr createGraph();
void loadBuildingsAndDistances(graphPtr graph);;
int minDistance(unsigned int dist[], unsigned int sptSet[]);
void printPath(int parent[], int j, graphPtr graph);
void printResult(unsigned int dist[], int parent[], graphPtr graph, int src, int dest, bool printPath);
void dijkstra(graphPtr graph, char source[], char destination[], bool printPath);
void topologicalSort( coursesGraphPtr graph );

int main() {
    coursesGraphPtr coursesGraph = createCoursesGraph();
    graphPtr graph = createGraph();

    int isDataLoaded = 0;

    // will hold the source and dest cities:
    char srcBuilding[20];
    char desBuilding[20];
    bool printPath=0;// 0 Don't print, 1 Print
    int selection = 0;
    do {
        printf("1- Load data from the files.\n");
        printf("2- Calculate the shortest distance between two buildings.\n");
        printf("3- Print the shortest route between two buildings and the total distance.\n");
        printf("4- Sort the courses using the topological sort and print result.\n");
        printf("5- Exit the application.\n");
        scanf("%d", &selection);
        if (selection == 1){
            //loads the file and construct the graph:
            loadBuildingsAndDistances(graph);
            isDataLoaded = 1;
            printf("Data is loaded successfully:)\n");
        }
        else if(selection == 2){
            if (!isDataLoaded){
                printf("You must load data from the files first!!\n");
                continue;
            }
            //read the source city:
            printf("Source building:\n-");
            scanf("%s", srcBuilding);
            printf("Destination building:\n-");
            scanf("%s", desBuilding);
            dijkstra(graph, srcBuilding, desBuilding, printPath);
        }
        else if(selection == 3){
            if (!isDataLoaded){
                printf("You must load data from the files first!!\n");
                continue;
            }
            //read the source city:
            printf("Source building:\n-");
            scanf("%s", srcBuilding);
            printf("Destination building:\n-");
            scanf("%s", desBuilding);
            printPath = 1;
            dijkstra(graph, srcBuilding, desBuilding, printPath);
        }
        else if(selection == 4){
            loadCourses(coursesGraph);
            topologicalSort(coursesGraph);
        }
        else if(selection == 5){
            free(graph);
            free(coursesGraph);
            exit(0);
        }
        else{
            printf("Invalid Selection:(");
        }
    } while (1);
}
void loadCourses(coursesGraphPtr graph) {

    FILE *file = fopen("../Courses.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[40];
    while (fgets(line, sizeof(line), file)) {
        queueNodePtr List = createCourseList();
        // Remove newline character from the end of the line
        line[strlen(line)-1] = '\0';
        // Split the line into fields using the delimiter
        char *token = strtok(line, "#");
        while (token != NULL) {
            // printf("%s  \n", token);
            insertCourseList(List, token);
            token = strtok(NULL, "#");
        }
        insertCourseGraph(graph, List);
        free(List);
    }
    fclose(file);
}
coursesGraphPtr createCoursesGraph(){
    coursesGraphPtr graph = (coursesGraphPtr)malloc(sizeof(struct graphCourses));

    if (graph == NULL) {
        printf("Out of memory!\n");
        return NULL;
    }
    for(int i = 0; i<maxNumOfCourses; i++){
        strcpy(graph->Courses[i], "no");
    }
    for (int i =0; i<maxNumOfCourses; i++){
        graph->CoursesGraph[i] = createCourseList();
    }
    return graph;
}
queuePtr createCoursesQueue()
{
    /* Allocate memory for the queue */
    queuePtr q = (queuePtr)malloc(sizeof(struct queue));

    if (q == NULL)
    {
        printf("Could not allocate queue. Memory is full.\n");
        exit(0);
    }
    q->front = q->rear = NULL;
    return q;
}
queueNodePtr createCourseNode(char word[])
{
    /* Allocate memory for the queue node */
    queueNodePtr temp = (queueNodePtr)malloc(sizeof(struct queueNode));

    if (temp == NULL)
    {
        printf("Could not allocate a new word node. Memory allocation failed.\n");
        exit(0);
    }

    strcpy(temp->courseName, word);
    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}
queueNodePtr createCourseList()
{
    /* Allocate memory for the queue node */
    queueNodePtr temp = (queueNodePtr)malloc(sizeof(struct queueNode));

    if (temp == NULL)
    {
        printf("Could not allocate a new word node. Memory allocation failed.\n");
        exit(0);
    }

    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}
void insertCourseList(queueNodePtr L, char Course[]){// Insert first{
    // Allocate memory for the new queue node
    queueNodePtr temp = createCourseNode(Course);
    // Insert the new node at the beginning of the list
    temp->next = L->next;
    L->next = temp;


}
int getCourseIndex(coursesGraphPtr graph, char token []){// Used to get the right index for a string in the string array.
    for (int i=0; i<maxNumOfCourses; i++){
        if(strcmp(graph->Courses[i], token) == 0)
            return i;
    }
    return -1;
}
void insertCourseGraph(coursesGraphPtr graph, queueNodePtr list){

    if (graph == NULL)
    {
        printf("Linked List Not Found..\n");
        return;
    }
    else if (list == NULL)
    {
        printf("Courses List Not Found..\n");
        return;
    }
    else {
        queueNodePtr temp = list->next;
        while (temp!=NULL){
            for (int i = 0; i < maxNumOfCourses; i++) {// Fill courses array.

                if(strcmp(graph->Courses[i], temp->courseName) == 0 ){
                    break;
                }
                else if (strcmp(graph->Courses[i], temp->courseName)!= 0 && strcmp(graph->Courses[i], "no") != 0 ){
                    ;
                }
                else if (strcmp(graph->Courses[i], "no") == 0 ) {
                    strcpy(graph->Courses[i], temp->courseName);
                    break;
                }
            }
            temp = temp->next;
        }
        temp = list->next;
        while (temp->next != NULL) { // to get dependence subject.
            temp = temp->next;
        }
        queueNodePtr end = temp;
        temp = list->next;
        while (temp != end) {
            // Get dominant subject
            int index = getCourseIndex(graph, temp->courseName);
            if (index == -1) {
                printf("Parent course is not in the list\n");
                break;
            } else
                insertCourseList(graph->CoursesGraph[index], end->courseName);// Insert dependence subject in the right place
            temp = temp->next;
        }
    }
}
void printCourseGraph(coursesGraphPtr graph){
    if (graph == NULL){
        printf("graph Not found\n");
        return;
    }
    for(int i=0; i<maxNumOfCourses; i++){
        queueNodePtr temp = graph->CoursesGraph[i]->next;
        printf("%s -> ", graph->Courses[i]);
        while(temp!=NULL){
            printf("%s ->", temp->courseName);
            temp = temp->next;
        }
        printf("\n");
    }
}
void enqueue(queuePtr q, char word[]){
    /* Handle if the word is too big */
    if (strlen(word) > 20)
    {
        printf("The word you've entered is too big. The limit is 20 characters.\n");
        exit(0);
    }
    /* Creates a new word node with the word string provided */
    queueNodePtr temp = createCourseNode(word);
    /* Queue is empty */
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }
    /* Insert in front */
    q->front->prev = temp;
    temp->next = q->front;
    temp->prev = NULL;
    q->front = temp;
}
void dequeue(queuePtr q){ // Dequeue rear from queue
    /* Handle if queue is empty */
    if (q->front == NULL)
    {
        printf("Queue is empty :(.\n");
        return;
    }
    strcpy(tokenCommunicate, q->rear->courseName);
    if (q->front != q->rear)
    {
        queueNodePtr temp = q->rear;
        q->rear = q->rear->prev;
        q->rear->next = NULL;
        free(temp);
    }
    else
    {
        q->rear = NULL;
        q->front = NULL;
    }
    // if rear becomes NULL, change rear also to NULL.
    if (q->rear == NULL)
    {
        q->front = NULL;
    }
}
destBuildingsList createDestBuildingsList(){
    destBuildingsList L;
    L = (destBuildingsList)malloc(sizeof(struct destBuilding));
    if (L == NULL)
    {
        printf("Could not create list. List is not found..\n");
        return NULL;
    }
    else
    {
        L->next = NULL;
    }

    return L;
}// Return the head of a linked list, used for Creating the graph.
void insertBuildings(graphPtr graph, char insertedSrcBuilding[], char insertedDestBuilding[], int insertedDistance){
    ptrToDestBuilding newNode = (ptrToDestBuilding)malloc(sizeof(struct destBuilding));
    strcpy(newNode->buildingName, insertedDestBuilding); // Needed strcpy() to copy the inserted string to the node
    newNode->distance = insertedDistance;
    newNode->visited = 0;
    newNode->next = NULL;

    if (graph == NULL)
    {
        printf("Linked List Not Found..\n");
    }
    else if (newNode == NULL)
    {
        printf("Out of space..\n");
    }
    else
    {
        for (int i = 0; i<maxNumOfBuildings; i++){// Source building inside array
            if (strcmp(graph->destBuildings[i], insertedSrcBuilding ) == 0){
                break;
            }
            else if (strcmp(graph->destBuildings[i], "no") == 0 ){
                strcpy(graph->destBuildings[i], insertedSrcBuilding);
                break;
            }
        }
        for (int i = 0; i<maxNumOfBuildings; i++){// destination buildings inside array
            if (strcmp(graph->destBuildings[i], insertedDestBuilding ) == 0){
                break;
            }
            else if (strcmp(graph->destBuildings[i], "no") == 0 ){
                strcpy(graph->destBuildings[i], insertedDestBuilding);
                break;
            }
        }
        int index = getIndex(graph, insertedSrcBuilding);
        if (index == -1)
            printf("Out of bounds\n");
        else {
            if(graph->buildingsGraph[index]->next == NULL){
                newNode->next = graph->buildingsGraph[index]->next;
                graph->buildingsGraph[index]->next = newNode;
            }
            else{
                ptrToDestBuilding temp = graph->buildingsGraph[index]->next;

                while (temp->next != NULL)
                {
                    temp = temp->next;
                }

                newNode->next = temp->next;
                temp->next = newNode;
            }
        }
    }
}
graphPtr createGraph(){
    graphPtr graph = (graphPtr)malloc(sizeof(struct graph));

    if (graph == NULL) {
        printf("Out of memory!\n");
        return NULL;
    }
    for(int i = 0; i<maxNumOfBuildings; i++){
        strcpy(graph->destBuildings[i], "no");
    }
    for (int i =0; i<maxNumOfBuildings; i++){
        graph->buildingsGraph[i] = createDestBuildingsList();
    }
    return graph;
}
int getIndex(graphPtr graph, char token []){// Used to get the index of the inserted string using the string array.
    for (int i=0; i<maxNumOfBuildings; i++){
        if(strcmp(graph->destBuildings[i], token) == 0)
            return i;
    }
    return -1;
}
void loadBuildingsAndDistances(graphPtr graph){

    // read data from the file
    FILE *file = fopen("../buildings.txt", "r");
    char line[46];// The maximum size of two buildings and distance with "#".
    if (file == NULL){
        printf("Error opening the file.\n");
    }
    char srcCity[20];
    char disCity[20];
    char distance[4];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strlen(line)-1] = '\0';// To remove \n from words
        int i = 0;
        while(line[i] != '#') {
            srcCity[i] = line[i];
            i++;
        }
        srcCity[i] = '\0';
        i++;
        int disCityIndex = 0;
        while(line[i] != '#') {
            disCity[disCityIndex] = line[i];
            i++;
            disCityIndex++;
        }
        disCity[disCityIndex] ='\0';
        i++;
        int disIndex = 0;
        while(line[i] != '\0') {
            distance[disIndex] = line[i];
            i++;
            disIndex++;
        }
        distance[disIndex] = line[i];

        int dist = atoi(distance);
        insertBuildings(graph, srcCity, disCity, dist);
    }
    fclose(file);
}
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