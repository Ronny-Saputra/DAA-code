#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct {
    int city;
    int time;
} Node;

typedef struct {
    Node* array;
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->array = (Node*)malloc(capacity * sizeof(Node));
    return pq;
}

void swap(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void insert(PriorityQueue* pq, int city, int time) {
    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        pq->array = (Node*)realloc(pq->array, pq->capacity * sizeof(Node));
    }

    pq->array[pq->size].city = city;
    pq->array[pq->size].time = time;
    pq->size++;

    // Up-heap bubble
    int i = pq->size - 1;
    while (i && pq->array[i].time < pq->array[(i - 1) / 2].time) {
        swap(&pq->array[i], &pq->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Node extractMin(PriorityQueue* pq) {
    if (pq->size == 0) {
        return (Node){-1, INT_MAX};
    }

    Node root = pq->array[0];
    pq->array[0] = pq->array[pq->size - 1];
    pq->size--;

    // Down-heap bubble
    int i = 0, smallest = 0;
    while (i < pq->size) {
        smallest = i;
        if (2 * i + 1 < pq->size && pq->array[smallest].time > pq->array[2 * i + 1].time) {
            smallest = 2 * i + 1;
        }
        if (2 * i + 2 < pq->size && pq->array[smallest].time > pq->array[2 * i + 2].time) {
            smallest = 2 * i + 2;
        }
        if (smallest == i) {
            break;
        }
        swap(&pq->array[i], &pq->array[smallest]);
        i = smallest;
    }

    return root;
}

void dijkstra(int** graph, int start, int numCities) {
    int* times = (int*)malloc((numCities + 1) * sizeof(int));
    bool* visited = (bool*)malloc((numCities + 1) * sizeof(bool));
    PriorityQueue* pq = createPriorityQueue(numCities);

    // Initialize all times as INFINITE and visited[] as false
    for (int i = 1; i <= numCities; i++) {
        times[i] = INT_MAX;
        visited[i] = false;
    }

    // Distance to the start node is zero
    times[start] = 0;
    insert(pq, start, 0);

    while (pq->size > 0) {
        Node node = extractMin(pq);
        int u = node.city;

        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 1; v <= numCities; v++) {
            if (graph[u][v] && !visited[v] && times[u] != INT_MAX &&
                times[u] + graph[u][v] < times[v]) {
                times[v] = times[u] + graph[u][v];
                insert(pq, v, times[v]);
            }
        }
    }

    // Display the results
    printf("City\tShortest Time from Start\n");
    for (int i = 1; i <= numCities; i++) {
        printf("%d\t%d\n", i, times[i]);
    }

    free(times);
    free(visited);
    free(pq->array);
    free(pq);
}

int main() {
    int numCities, start;

    printf("Enter the number of cities: ");
    scanf("%d", &numCities);

    // Allocate memory for the graph dynamically
    int** graph = (int**)malloc((numCities + 1) * sizeof(int*));
    for (int i = 0; i <= numCities; i++) {
        graph[i] = (int*)malloc((numCities + 1) * sizeof(int));
    }

    // Input city names
    char** cityNames = (char**)malloc((numCities + 1) * sizeof(char*));
    for (int i = 1; i <= numCities; i++) {
        cityNames[i] = (char*)malloc(50 * sizeof(char)); // 50 characters for city names
        printf("Enter name for city %d: ", i);
        scanf("%s", cityNames[i]);
    }

    // Input distances
    printf("Enter the distances between cities (0 if no direct route):\n");
    for (int i = 1; i <= numCities; i++) {
        for (int j = 1; j <= numCities; j++) {
            if (i != j) {
                printf("Distance from %s to %s: ", cityNames[i], cityNames[j]);
                scanf("%d", &graph[i][j]);
            } else {
                graph[i][j] = 0; // Distance to itself is 0
            }
        }
    }

    printf("Enter the starting city (1 to %d): ", numCities);
    scanf("%d", &start);

    // Run Dijkstra's algorithm
    dijkstra(graph, start, numCities);

    // Free allocated memory
    for (int i = 0; i <= numCities; i++) {
        free(graph[i]);
    }
    free(graph);

    for (int i = 1; i <= numCities; i++) {
        free(cityNames[i]);
    }
    free(cityNames);

    return 0;
}
