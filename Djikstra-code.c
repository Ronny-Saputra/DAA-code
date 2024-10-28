#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define NUM_CITIES 5  // Define the number of cities (nodes)

int findMinTime(int times[], bool visited[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < NUM_CITIES; v++) {
        if (!visited[v] && times[v] <= min) {
            min = times[v];
            min_index = v;
        }
    }

    return min_index;
}

void dijkstra(int graph[NUM_CITIES][NUM_CITIES], int start) {
    int times[NUM_CITIES];       // Array to hold the shortest travel times
    bool visited[NUM_CITIES];    // Array to mark visited nodes

    // Initialize all times as INFINITE and visited[] as false
    for (int i = 0; i < NUM_CITIES; i++) {
        times[i] = INT_MAX;
        visited[i] = false;
    }

    // Distance to the start node is zero
    times[start] = 0;

    // Find the shortest path for all cities
    for (int count = 0; count < NUM_CITIES - 1; count++) {
        int u = findMinTime(times, visited);
        visited[u] = true;

        // Update times value for adjacent cities
        for (int v = 0; v < NUM_CITIES; v++) {
            if (!visited[v] && graph[u][v] && times[u] != INT_MAX 
                && times[u] + graph[u][v] < times[v]) {
                times[v] = times[u] + graph[u][v];
            }
        }
    }

    // Display the results
    printf("City\tShortest Time from Start\n");
    for (int i = 0; i < NUM_CITIES; i++) {
        printf("%d\t%d\n", i, times[i]);
    }
}

int main() {
    // Example graph: adjacency matrix where 0 means no direct flight
    int graph[NUM_CITIES][NUM_CITIES] = {
        {0, 10, 20, 0, 0},
        {10, 0, 5, 15, 0},
        {20, 5, 0, 10, 5},
        {0, 15, 10, 0, 25},
        {0, 0, 5, 25, 0}
    };

    int start = 0;  // Define the start city
    dijkstra(graph, start);

    return 0;
}
