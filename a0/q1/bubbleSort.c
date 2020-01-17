#include <stdio.h> 
  
void swap(float *xp, float *yp) 
{ 
    float temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
// A function to implement bubble sort 
void bubbleSort(float arr[], int n) 
{ 
   int i, j; 
   for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
} 
  
/* Function to print an array */
void printArray(float arr[], int size) 
{ 
    int i; 
    for (i=0; i < size; i++) 
        printf("%f ", arr[i]); 
    printf("\n"); 
} 
  
// Driver program to test above functions 
int main() 
{ 
    float arr[] = {6.4, 25.5, 25.6, 2.5, 2.3, 1, 0.5}; 
    int n = sizeof(arr)/sizeof(arr[0]); 
    bubbleSort(arr, n); 
    printf("Sorted array: \n"); 
    printArray(arr, n); 
    return 0; 
} 
