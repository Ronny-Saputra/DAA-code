# Dijkstra's Algorithm Implementation in C

This project implements Dijkstra's algorithm for finding the shortest path from a starting city to all other cities in a graph. The implementation includes dynamic memory allocation, a priority queue for efficient node extraction, and allows the user to define city names.

## Features

- Dynamic handling of cities (no predefined limit other than memory).
- Priority queue implementation for efficient time complexity.
- User-defined city names for better clarity.
- Interactive input for distances and starting city.

## Requirements

- A C compiler (GCC, Clang, etc.)
- Standard libraries: `stdio.h`, `stdlib.h`, `limits.h`, `stdbool.h`

## Usage

1. Clone the repository or download the source code.
2. Open a terminal and navigate to the directory containing the source code.
3. Compile the code using the following command:
   ```bash
   gcc DijkstraFlight.c -o dijkstra
