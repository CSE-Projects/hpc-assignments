#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const int n = 1000;

int main() {
    srand(1);

    int m[n][n];
    long double cur_lambda, b[n], v[n];

    for(int i = 0; i < n; ++i) {
    
        int random_b = rand() % n;
        b[i] = random_b;
    
        for(int j = i; j < n; ++j) {
            int random = rand() % 2;
            m[i][j] = m[j][i] = random;
        }
    }

    // Print matrix
    // for(int i = 0; i < n; ++i) {
    //     for(int j = 0; j < n; ++j) {
    //         printf("%d ", m[i][j]);
    //     }
    //     printf("\n");
    // }

    // cur_lambda
    long double sum = 0;
    for(int i = 0; i < n; ++i) {
        sum += b[i] * b[i];
    }
    cur_lambda = sqrt(sum);
    for(int i = 0; i < n; ++i) {
        v[i] = b[i] / cur_lambda;
    }
    for(int i = 0; i < n; ++i) {
        
        long double addition = 0;
        
        for(int j = 0; j < n; ++j) {
            // addition += m[j][i] * v[j]; 
            addition += m[i][j] * v[j]; 
        }

        b[i] = addition;
    }

    while(1) {
        long double new_lambda;

        long double add = 0;
        for(int i = 0; i < n; ++i) {
            add += b[i] * b[i];
        }
        new_lambda = sqrt(add);
        for(int i = 0; i < n; ++i) {
            v[i] = b[i] / new_lambda;
        }
        for(int i = 0; i < n; ++i) {
            
            long double addition = 0;
            
            for(int j = 0; j < n; ++j) {
                // addition += m[j][i] * v[j];  
                addition += m[i][j] * v[j]; 
            }

            b[i] = addition;
        }

        if(fabs(new_lambda - cur_lambda) < 1e-6) {
            break;
        }
        else {
            cur_lambda = new_lambda;
        }
    }

    printf("%Lf", cur_lambda);

    return 0;
}