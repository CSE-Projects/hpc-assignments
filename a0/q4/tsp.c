#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define N 10

int distance[N][N], nodes_visited[N];

int travellingSalesmanProblem(int curCity) {
    nodes_visited[curCity] = 1;

    int minimum = INT_MAX;
    for(int i = 0; i < N; ++i) {
        if(nodes_visited[i] == 0) {
            int cost = travellingSalesmanProblem(i) + distance[curCity][i];
            if(cost < minimum) {
                minimum = cost;
            }
        }
    }

    nodes_visited[curCity] = 0;

    // Base case
    if(minimum == INT_MAX) {
        return distance[curCity][0];
    }
    return minimum;
}

int main() {
    srand(1);

    printf("Number of Cities: %d\n", N);
    
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if(i == j) distance[i][j] = 0;
            else distance[i][j] = rand() % 100 + 1;
        }
    }

    // Print matrix

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            printf("%d ", distance[i][j]);
        }
        printf("\n");
    }

    printf("Cost: %d\n", travellingSalesmanProblem(0));
    return 0;
}