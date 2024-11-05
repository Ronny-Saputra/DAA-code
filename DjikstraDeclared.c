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

void dijkstra(int graph[19][19], int start, int numCities, char** cityNames){
    int* times = (int*)malloc(numCities * sizeof(int));
    bool* visited = (bool*)malloc(numCities * sizeof(bool));
    PriorityQueue* pq = createPriorityQueue(numCities);

    for (int i = 0; i < numCities; i++) {
        times[i] = INT_MAX;
        visited[i] = false;
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
                insert(pq, v, times[v]);
            }
        }
    }

    printf("City\t\tShortest Time from Start\n");
    for (int i = 0; i < numCities; i++) {
        printf("%s\t%d\n", cityNames[i], times[i]);
    }

    free(times);
    free(visited);
    free(pq->array);
    free(pq);
}

int main() {
    int numCities = 19;
    int start;

    char* cityNames[] = {
        "Jakarta (CGK)", "Surabaya", "Denpasar", "Makassar", "Medan", "Pekanbaru",
        "Palembang", "Semarang", "Yogyakarta", "Praya, Lombok", "Labuan Bajo",
        "Kupang", "Ambon", "Manado", "Ternate", "Sorong", "Jayapura",
        "Pontianak", "Balikpapan", "Banjarmasin"
    };

    int graph[19][19] = {
    {0, 429, 610, 889, 861, 579, 260, 188, 353, 256, 491, 767, 1081, 1039, 1154, 1222, 0, 550, 507},
    {429, 0, 188, 491, 1224, 950, 636, 350, 0, 75, 322, 587, 0, 978, 676, 847, 0, 0, 530},
    {610, 188, 0, 394, 0, 0, 0, 0, 679, 340, 0, 451, 595, 588, 0, 847, 0, 0, 0},
    {889, 491, 394, 0, 0, 0, 0, 0, 644, 0, 0, 451, 595, 588, 676, 847, 1454, 0, 319},
    {861, 1224, 0, 0, 0, 282, 604, 1079, 1109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {579, 950, 0, 0, 282, 0, 0, 0, 828, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {260, 636, 0, 0, 604, 0, 0, 188, 353, 0, 491, 767, 1081, 1039, 1154, 1222, 0, 550, 507},
    {188, 350, 0, 0, 1079, 0, 188, 0, 353, 256, 491, 767, 1081, 1039, 1154, 1222, 0, 550, 507},
    {353, 0, 679, 644, 1109, 0, 828, 353, 0, 0, 451, 767, 1081, 1039, 1154, 1222, 0, 550, 507},
    {256, 75, 340, 0, 0, 0, 0, 256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 550, 507},
    {491, 322, 0, 0, 0, 0, 491, 491, 451, 0, 0, 0, 0, 0, 0, 0, 0, 550, 507},
    {767, 587, 451, 451, 0, 0, 767, 767, 767, 0, 0, 0, 0, 0, 0, 0, 0, 550, 507},
    {1081, 0, 595, 595, 0, 0, 1081, 1081, 1081, 0, 0, 0, 0, 0, 0, 0, 0, 550, 507},
    {1039, 978, 588, 588, 0, 0, 1039, 1039, 1039, 0, 0, 0, 0, 0, 0, 0, 0, 550, 507},
    {1154, 676, 0, 676, 0, 0, 1154, 1154, 1154, 0, 0, 0, 0, 0, 0, 0, 0, 550, 507},
    {1222, 847, 847, 847, 0, 0, 1222, 1222, 1222, 0, 0, 0, 0, 0, 0, 0, 0, 550, 507},
    {0, 0, 0, 1454, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 550, 507},
    {550, 0, 0, 0, 0, 0, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 0, 0},
    {507, 530, 0, 319, 0, 0, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 0, 0}
};

    // Tampilkan daftar kota untuk pemilihan kota awal
    printf("Pilih kota awal:\n");
    for (int i = 0; i < numCities; i++) {
        printf("%d. %s\n", i + 1, cityNames[i]);
    }
    printf("Masukkan nomor kota awal (1-%d): ", numCities);
    scanf("%d", &start);
    start--; // Sesuaikan ke indeks array

    // Jalankan algoritma Dijkstra
    dijkstra((int**)graph, start, numCities, cityNames);

    return 0;
}