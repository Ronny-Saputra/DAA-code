#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_CITIES 10  // Define the maximum number of cities

int findMinTime(int times[], bool visited[], int numCities) {
    int min = INT_MAX, min_index;

    for (int v = 1; v <= numCities; v++) {
        if (!visited[v] && times[v] <= min) {
            min = times[v];
            min_index = v;
        }
    }

    return min_index;
}

void dijkstra(int graph[MAX_CITIES + 1][MAX_CITIES + 1], int start, int numCities) {
    int times[MAX_CITIES + 1];
    bool visited[MAX_CITIES + 1];

    // Initialize all times as INFINITE and visited[] as false
    for (int i = 1; i <= numCities; i++) {
        times[i] = INT_MAX;
        visited[i] = false;
    }

    // Distance to the start node is zero
    times[start] = 0;

    // Find the shortest path for all cities
    for (int count = 1; count < numCities; count++) {
        int u = findMinTime(times, visited, numCities);
        visited[u] = true;

        for (int v = 1; v <= numCities; v++) {
            if (!visited[v] && graph[u][v] && times[u] != INT_MAX 
                && times[u] + graph[u][v] < times[v]) {
                times[v] = times[u] + graph[u][v];
            }
        }
    }

    // Display the results
    printf("City\tShortest Time from Start\n");
    for (int i = 1; i <= numCities; i++) {
        printf("%d\t%d\n", i, times[i]);
    }
}

int main() {
    int numCities, start;
    int graph[MAX_CITIES + 1][MAX_CITIES + 1];  // +1 to index from 1

    printf("Enter the number of cities (max %d): ", MAX_CITIES);
    scanf("%d", &numCities);

    if (numCities > MAX_CITIES) {
        printf("Number of cities exceeds the maximum limit of %d.\n", MAX_CITIES);
        return 1;
    }

    // Input distances
    printf("Enter the distances between cities (0 if no direct route):\n");
    for (int i = 1; i <= numCities; i++) {
        for (int j = 1; j <= numCities; j++) {
            if (i != j) {
                printf("Distance from city %d to city %d: ", i, j);
                scanf("%d", &graph[i][j]);
            } else {
                graph[i][j] = 0;
            }
        }
    }

    printf("Enter the starting city (1 to %d): ", numCities);
    scanf("%d", &start);

    // Run Dijkstra's algorithm
    dijkstra(graph, start, numCities);

    return 0;
}
