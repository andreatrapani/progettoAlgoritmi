#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

int findDuplicate(int connections[], int numNodes) {
    // Array to track already seen elements
    bool seen[numNodes];  // Use numNodes as the array size
    for (int i = 0; i < numNodes; i++) {
        seen[i] = false;
    }

    for (int i = 0; i < numNodes; i++) {
        if (connections[i] < 0 || connections[i] >= numNodes) {
            return -1;  // Check for out-of-bound values
        }
        if (seen[connections[i]]) {
            return connections[i];  // Duplicate found
        }
        seen[connections[i]] = true;
    }
    return -1;
}

int findMinOf(int connections[], int values[], int duplicate, int numNodes) {
    int minIndex = -1;
    int minValue = INT_MAX;

    for (int i = 0; i < numNodes; i++) {
        if (connections[i] == duplicate && values[i] < minValue) {
            minValue = values[i];
            minIndex = i;
        }
    }

    return minIndex;
}

void changeValue(int connections[], int index, int numNodes) {
    // Auxiliary array to track existing values
    bool used[numNodes];
    for (int i = 0; i < numNodes; i++) {
        used[i] = false;
    }

    // Mark as true the values already present in the connections array
    for (int i = 0; i < numNodes; i++) {
        if (connections[i] >= 0 && connections[i] < numNodes) {
            used[connections[i]] = true;
        }
    }

    // Find the first unused value
    for (int value = 0; value < numNodes; value++) {
        if (!used[value] && value != (index + 1)) {
            connections[index] = value;
            return;
        }
    }
}

int calculateCost(int connections[], int values[], int numNodes) {
    int cost = 0, duplicate;
    while ((duplicate = findDuplicate(connections, numNodes)) != -1) {
        int indexOfDuplicate = findMinOf(connections, values, duplicate, numNodes);
        changeValue(connections, indexOfDuplicate, numNodes);
        cost += values[indexOfDuplicate];
    }
    return cost;
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