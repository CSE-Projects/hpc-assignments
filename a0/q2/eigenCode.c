#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define INF 100000

const int n = 3;

int main() {

    srand(1);
    double cur_lambda, b[n], v[n], m[n][n];

    for(int i = 0; i < n; ++i) {
    
        int random_b = rand() % INF;
        b[i] = random_b;
    
        for(int j = i; j < n; ++j) {
            int random = (rand() % INF) + 1;
    
            // random [0, 1]
            m[i][j] = m[j][i] = 1.0 / random;
        }
    }

    // Print matrix
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }

    // cur_lambda
    double sum = 0;
    for(int i = 0; i < n; ++i) {
        sum += b[i] * b[i];
    }
    cur_lambda = sqrtl(sum);
    for(int i = 0; i < n; ++i) {
        v[i] = b[i] / cur_lambda;
    }
    for(int i = 0; i < n; ++i) {
        
        double addition = 0;
        
        for(int j = 0; j < n; ++j) {
            // addition += m[j][i] * v[j]; 
            addition += m[i][j] * v[j]; 
        }

        b[i] = addition;
    }

    while(1) {
        double new_lambda;

        double add = 0;
        for(int i = 0; i < n; ++i) {
            add += b[i] * b[i];
        }
        new_lambda = sqrtl(add);
        
        for(int i = 0; i < n; ++i) {
            v[i] = b[i] / new_lambda;
        }
        for(int i = 0; i < n; ++i) {
            
            double addition = 0;
            
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

    printf("%lf", cur_lambda);

    return 0;
}