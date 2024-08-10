#include <stdio.h>


int potential(int binary[], int size) {
    int phi = 0;
    for (int i = 0; i < size; i++) {
        if (binary[i] == 1) {
            phi++;
        }
    }
    return phi;
}

int main() {
    int size = 4;
    int binary[] = {0, 0, 0, 0}; 
    int n = 16; 
    int actualCost = 0;
    int amortizedCost = 0;

    
    int initialPotential = potential(binary, size);

    for (int count = 1; count <= n; count++) {
        int actual = 0;
        
        
        for (int i = 0; i < size; i++) {
            if (binary[i] == 0) {
                binary[i] = 1;
                actual++;
                break;
            } else {
                binary[i] = 0;
                actual++;
            }
        }
        
        int newPotential = potential(binary, size);

        
        amortizedCost += actual + (newPotential - initialPotential);
        
        
        initialPotential = newPotential;

    
        actualCost += actual;

        
        printf("Count: %2d | Binary: ", count);
        for (int i = size - 1; i >= 0; i--) {
            printf("%d", binary[i]);
        }
        printf(" | Actual: %d | Amortized: %d\n", actual, amortizedCost);
    }

    printf("Total Actual Cost: %d\n", actualCost);
    printf("Total Amortized Cost: %d\n", amortizedCost);

    return 0;
}
