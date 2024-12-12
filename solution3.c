#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

int calculateCost(int connections[], int values[], int numNodes) {
    int max = values[0], cost=values[0];
    
    for (int i = 1; i < numNodes; i++)
    {
        cost += values[i];
        if(connections[i] == connections[i-1]){
            if(values[i] > max)
                max = values[i];
        }else{
            cost -= max;
            max = values[i];
        }
    }
    return cost-max;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    int numNodes;
    if (fscanf(file, "%d", &numNodes) != 1) {
        fprintf(stderr, "Error reading the number of nodes.\n");
        fclose(file);
        return 1;
    }

    int connections[numNodes];
    int values[numNodes];

    for (int i = 0; i < numNodes; i++) {
        if (fscanf(file, "%d", &connections[i]) != 1) {
            fprintf(stderr, "Error reading connections.\n");
            fclose(file);
            return 1;
        }
    }

    for (int i = 0; i < numNodes; i++) {
        if (fscanf(file, "%d", &values[i]) != 1) {
            fprintf(stderr, "Error reading values.\n");
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    // Time measurement
    clock_t start, end;
    start = clock();

    int cost = calculateCost(connections, values, numNodes);

    end = clock();

    // Calculate execution time in seconds
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("COST: %d\n", cost);
    printf("Execution Time: %.5f sec\n", time_taken);

    return 0;
}