#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

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

void printPath(int* parent, int j, char** cityNames) {
    if (parent[j] == -1) {
        printf("%s", cityNames[j]);
        return;
    }
    printPath(parent, parent[j], cityNames);
    printf(" -> %s", cityNames[j]);
}

void dijkstra(int graph[20][20], int start, int numCities, char** cityNames) {
    int* times = (int*)malloc(numCities * sizeof(int));
    bool* visited = (bool*)malloc(numCities * sizeof(bool));
    int* parent = (int*)malloc(numCities * sizeof(int));
    PriorityQueue* pq = createPriorityQueue(numCities);

    for (int i = 0; i < numCities; i++) {
        times[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1;  // Inisialisasi parent ke -1
    }

    times[start] = 0;
    insert(pq, start, 0);

    while (pq->size > 0) {
        Node node = extractMin(pq);
        int u = node.city;

        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 0; v < numCities; v++) {
            if (graph[u][v] && !visited[v] && times[u] != INT_MAX &&
                times[u] + graph[u][v] < times[v]) {
                times[v] = times[u] + graph[u][v];
                parent[v] = u;  // Simpan kota sebelumnya dalam jalur
                insert(pq, v, times[v]);
            }
        }
    }

    // Cetak hasil
    printf("City\t\tShortest Time from Start\tPath\n");
    for (int i = 0; i < numCities; i++) {
        printf("%s\t%d\t\t\t", cityNames[i], times[i]);
        printPath(parent, i, cityNames);
        printf("\n");
    }

    free(times);
    free(visited);
    free(parent);
    free(pq->array);
    free(pq);
}

int main() {
    int numCities = 20;
    int start;

    char* cityNames[] = {
        "Jakarta (CGK)", "Surabaya", "Denpasar", "Makassar", "Medan", "Pekanbaru",
        "Palembang", "Semarang", "Yogyakarta", "Praya, Lombok", "Labuan Bajo",
        "Kupang", "Ambon", "Manado", "Ternate", "Sorong", "Jayapura",
        "Pontianak", "Balikpapan", "Banjarmasin"
    };

    int graph[20][20] = {
    {0, 429, 610, 889, 861, 579, 260, 261, 264, 683, 920, 1195, 1483, 1366, 1507, 1723, 2343, 454, 781, 587},
    {429, 0, 188, 491, 1224, 950, 636, 188, 0, 256, 491, 767, 1081, 1039, 1154, 0, 0, 550, 507, 303},
    {610, 188, 0, 394, 0, 0, 0, 350, 353, 75, 322, 587, 0, 978, 0, 1222, 0, 0, 530, 367},
    {889, 491, 394, 0, 0, 0, 0, 644, 679, 340, 0, 451, 595, 588, 676, 847, 1454, 0, 319, 348},
    {861, 1224, 0, 0, 0, 282, 604, 1079, 1109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {579, 950, 0, 0, 282, 0, 0, 0, 828, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {260, 636, 0, 0, 604, 0, 0, 481, 505, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {261, 188, 350, 644, 1079, 0, 481, 0, 0, 422, 0, 0, 0, 0, 0, 0, 0, 0, 597, 387},
    {264, 0, 353, 679, 1109, 828, 505, 0, 0, 428, 0, 0, 0, 0, 0, 0, 0, 537, 656, 446},
    {683, 256, 75, 340, 0, 0, 0, 422, 428, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {920, 491, 322, 0, 0, 0, 0, 0, 0, 0, 0, 287, 0, 0, 0, 0, 0, 0, 0, 0},
    {1195, 767, 587, 451, 0, 0, 0, 0, 0, 0, 287, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1483, 1081, 0, 595, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 317, 284, 0, 0, 0, 0},
    {1366, 1039, 978, 588, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 176, 460, 0, 0, 587, 0},
    {1507, 1154, 0, 676, 0, 0, 0, 0, 0, 0, 0, 0, 317, 176, 0, 0, 0, 0, 0, 0},
    {1723, 0, 1222, 847, 0, 0, 0, 0, 0, 0, 0, 0, 284, 460, 0, 0, 658, 0, 0, 0},
    {2343, 0, 0, 1454, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 658, 0, 0, 0, 0},
    {454, 550, 0, 0, 0, 0, 0, 0, 537, 0, 0, 0, 0, 0, 0, 0, 0, 0, 522, 0},
    {781, 507, 530, 319, 0, 0, 0, 597, 656, 0, 0, 0, 0, 587, 0, 0, 0, 522, 0, 210},
    {587, 303, 367, 348, 0, 0, 0, 387, 446, 0, 0, 0, 0, 0, 0, 0, 0, 0, 210, 0},
};


    printf("Pilih kota awal:\n");
    for (int i = 0; i < numCities; i++) {
        printf("%d. %s\n", i + 1, cityNames[i]);
    }
    printf("Masukkan nomor kota awal (1-%d): ", numCities);
    scanf("%d", &start);
    start--;

    clock_t start_time = clock();
    dijkstra((int**)graph, start, numCities, cityNames);
    clock_t end_time = clock();

    double running_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Running time: %.12f seconds\n", running_time);

    return 0;
}
