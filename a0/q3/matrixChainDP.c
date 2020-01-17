#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
  
#define N 15

int visited[N][N], dp[N][N];

int matrixChainDP(int array[], int low, int high) { 
    if(visited[low][high] != 0) {
        return dp[low][high];
    }
    
    if(low == high) { 
        return 0; 
    }
    int ret = INT_MAX; 

    // low-i & i+1-high
    for(int i = low; i+1 <= high; ++i) { 
        int cur_value = matrixChainDP(array, low, i) +  
                        matrixChainDP(array, i+1, high) + 
                        array[low]*array[i+1]*array[high+1]; 
        if (cur_value < ret) { 
            ret = cur_value; 
        }
    } 

    visited[low][high] = 1;
    dp[low][high] = ret;
    return ret; 
} 
  
int main() {
    srand(2);
    int array[N];
    for(int i = 0; i < N; ++i) {
        array[i] = rand() % 100;
        for(int j = 0; j < N; ++j) {
            visited[i][j] = 0;
        }
    }
    printf("Operations required: %d\n", matrixChainDP(array, 0, N-2));
    return 0; 
} 