#ifndef FULL_CODE_H
#define FULL_CODE_H

#include <Arduino.h> 

const int NUM_CHECKPOINTS = 6;
const int INF = 9999;  // Represents no path (infinity)

// Graph representation of the track
/*
int graph1[NUM_CHECKPOINTS][NUM_CHECKPOINTS] = {
  {  0,   7,   8, INF,   4 }, // From 0
  {  7,   0,   7,   11,   11 }, // From 1
  {  8,   7,   0,   4, INF }, // From 2
  {INF,   11,   4,   0,   19 }, // From 3
  {  4,   11, INF,   19,   0 }, // From 4
};
*/

int graph[NUM_CHECKPOINTS][NUM_CHECKPOINTS] = {
  {  0,   7,   8, INF,   4, INF }, // From 0
  {  7,   0,   7,   11,   11, 2 }, // From 1
  {  8,   7,   0,   4, INF, INF }, // From 2
  {INF,   11,   4,   0,   19, 9 }, // From 3
  {  4,   11, INF,   19,   0, 9 }, // From 4
  {INF,   2,   INF,  9,    9, 0 }, // From 6 (referred to as 5 in terms of indexing)         
};

int findMinDistance(int distance[], bool visited[]) {
  int min = INF, minIndex = -1;

  for (int i = 0; i < NUM_CHECKPOINTS; i++) {
    if (!visited[i] && distance[i] < min) {
      min = distance[i];
      minIndex = i;
    }
  }
  return minIndex;
}

// Dijkstra’s algorithm implementation
void dijkstra(int graph[NUM_CHECKPOINTS][NUM_CHECKPOINTS], int start, int target, int path[], int spacer) {
  int distance[NUM_CHECKPOINTS];
  bool visited[NUM_CHECKPOINTS];
  int prev[NUM_CHECKPOINTS];

  for (int i = 0; i < NUM_CHECKPOINTS; i++) {
    distance[i] = INF;
    visited[i] = false;
    prev[i] = -1;
  }
  distance[start] = 0;

  for (int count = 0; count < NUM_CHECKPOINTS - 1; count++) {
    int u = findMinDistance(distance, visited);
    if (u == -1) break;

    // Skip node 5 if spacer is active
    if (spacer == 2 && u == 5) continue;

    visited[u] = true;

    for (int v = 0; v < NUM_CHECKPOINTS; v++) {
      if (spacer == 2 && v == 5) continue;  // Skip updates involving node 5
      if (!visited[v] && graph[u][v] > 0 && distance[u] + graph[u][v] < distance[v]) {
        distance[v] = distance[u] + graph[u][v];
        prev[v] = u;
      }
    }
  }

  int current = target, index = 0;
  while (current != -1) {
      path[index++] = current;
      current = prev[current];
  }

  for (int i = 0; i < index / 2; i++) {
      int temp = path[i];
      path[i] = path[index - i - 1];
      path[index - i - 1] = temp;
  }

  path[index] = -1;
}


// Function to simulate obstacle blocking
void blockPath(int checkpointA, int checkpointB) {
  graph[checkpointA][checkpointB] = INF;
  graph[checkpointB][checkpointA] = INF;
  Serial.print("Blocked path between ");
  Serial.print(checkpointA);
  Serial.print(" and ");
  Serial.println(checkpointB);
}

// Function to print the path
void printPath(int path[]) {
  Serial.print("Path: ");
  for (int i = 0; path[i] != -1; i++) {
    Serial.print(path[i]);
 if (path[i + 1] != -1) Serial.print(" -> ");
  }
  Serial.println();
}

#endif

