#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
  
#define N 15

int matrixChainRecursive(int array[], int low, int high) { 
    if(low == high) { 
        return 0; 
    }
    int ret = INT_MAX; 

    // low-i & i+1-high
    for(int i = low; i+1 <= high; ++i) { 
        int cur_value = matrixChainRecursive(array, low, i) +  
                        matrixChainRecursive(array, i+1, high) + 
                        array[low]*array[i+1]*array[high+1]; 
        if (cur_value < ret) { 
            ret = cur_value; 
        }
    } 

    return ret; 
} 
  
int main() {
    srand(1);

    int array[N];
    for(int i = 0; i < N; ++i) {
        array[i] = rand() % 100;
    }
    printf("Operations required: %d\n", matrixChainRecursive(array, 0, N-2));
    return 0; 
} 