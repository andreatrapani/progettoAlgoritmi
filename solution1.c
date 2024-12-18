#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

// Definition of the node structure
typedef struct node {
    int value;            // Node value
    int index;            // Node index
    int numChildren;      // Current number of children
    int maxChildren;      // Maximum number of children
    struct node **children;  // Pointers to children
} Node;

// Recursive function to print the nodes of the tree - Soldani Marco
void printNodes(Node* node) {
    if (node == NULL) return;

    printf("node[%d] -> (%d) -> numChildren(%d) -> {", node->index, node->value, node->numChildren);
    for (int i = 0; i < node->numChildren; i++) {
        printf("node[%d]", node->children[i]->index);
        if (i < node->numChildren - 1) printf(", ");
    }
    printf("}\n");

    for (int i = 0; i < node->numChildren; i++) {
        printNodes(node->children[i]);
    }
}

// Function to find a node by its index - Vavassori Enrico
Node* findNode(Node* node, int index) {
    if (node == NULL) return NULL;
    if (node->index == index) return node;

    for (int x = 0; x < node->numChildren; x++) {
        Node *result = findNode(node->children[x], index);
        if (result != NULL) return result;
    }
    return NULL;
}

// Function to insert a node as a child - Trapani Andrea
bool insertNode(Node *nodeToInsert, Node *parentNode) {
    if (nodeToInsert == NULL || parentNode == NULL) return false;

    if (parentNode->numChildren >= parentNode->maxChildren) {
        fprintf(stderr, "Error: exceeded child limit for node %d.\n", parentNode->index);
        return false;
    }

    parentNode->children[parentNode->numChildren] = nodeToInsert;
    parentNode->numChildren++;
    return true;
}

// Function to create a new node - Trapani Andrea
Node* createNode(int value, int index, int maxChildren) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error: unable to allocate memory for the node.\n");
        return NULL;
    }

    node->value = value;
    node->index = index;
    node->numChildren = 0;
    node->maxChildren = maxChildren;

    if (maxChildren > 0) {
        node->children = (Node **)malloc(maxChildren * sizeof(Node *));
        if (node->children == NULL) {
            fprintf(stderr, "Error: unable to allocate memory for children.\n");
            free(node);
            return NULL;
        }
    } else {
        node->children = NULL;
    }

    return node;
}

// Function to calculate the number of children for a given index - Soldani Marco 
int countChildren(const int connections[], int numNodes, int index) {
    int numChildren = 0;
    for (int i = 0; i < numNodes; i++) {
        if (connections[i] == index) numChildren++;
    }
    return numChildren;
}

// Function to create the tree - Vavassori Enrico
Node* createTree(FILE *file, int *numNodes) {
    if (fscanf(file, "%d", numNodes) != 1) {
        fprintf(stderr, "Error reading the number of nodes.\n");
        return NULL;
    }

    int connections[*numNodes];
    int values[*numNodes];

    for (int i = 0; i < *numNodes; i++) {
        if (fscanf(file, "%d", &connections[i]) != 1) {
            fprintf(stderr, "Error reading connections.\n");
            return NULL;
        }
    }

    for (int i = 0; i < *numNodes; i++) {
        if (fscanf(file, "%d", &values[i]) != 1) {
            fprintf(stderr, "Error reading values.\n");
            return NULL;
        }
    }

    Node *root = createNode(0, 0, countChildren(connections, *numNodes, 0) + 1);
    if (root == NULL) return NULL;

    for (int i = 0; i < *numNodes; i++) {
        int numChildren = countChildren(connections, *numNodes, i + 1);
        Node *childNode = createNode(values[i], i + 1, numChildren + 1);

        if (childNode == NULL) return NULL;

        Node *parentNode = findNode(root, connections[i]);
        if (parentNode == NULL || !insertNode(childNode, parentNode)) {
            fprintf(stderr, "Error inserting node %d into parent %d.\n", childNode->index, connections[i]);
            return NULL;
        }
    }

    return root;
}

// Recursive function to free the tree - Trapani Andrea
void freeTree(Node *node) {
    if (node == NULL) return;

    // Free the children recursively
    for (int i = 0; i < node->numChildren; i++) {
        freeTree(node->children[i]);
    }

    // Free the array of children
    free(node->children);

    // Free the node itself
    free(node);
}

// Boolean function to determine if a node should be detached - Vavassori Enrico
bool isToDetach(Node *node) {
    if (node == NULL) return false;  // Base case: null node
    if (node->numChildren > 1) return true;  // Node with more than one child
    if (node->numChildren == 1) return isToDetach(node->children[0]);  // Recursion for the only child
    return false;  // Node without children
}

// Function to find the node to detach, which will then be moved under a leaf node - Trapani Andrea
Node* findNodeToDetach(Node *node) {
    int min = INT_MAX;
    int nodeIndex = 0;
    Node *nodeToDetach;
    if (node == NULL) return NULL;
    if (node->numChildren == 1) return findNodeToDetach(node->children[0]);

    for (int x = 0; x < node->numChildren; x++) {
        if (node->children[x]->value < min) {
            nodeToDetach = node->children[x];
            nodeIndex = x;
            min = node->children[x]->value;
        }
    }

    // Remove the child from the children array and decrease the count
    for (int i = nodeIndex; i < node->numChildren - 1; i++) {
        node->children[i] = node->children[i + 1];
    }
    node->children[node->numChildren - 1] = NULL;  // Cleanup
    node->numChildren--;
    return nodeToDetach;
}

//function that move the node keeping the left moving down recursively - Soldani Marco
void moveNode(Node *nodeToMove, Node *node) {
    if (nodeToMove == NULL || node == NULL) return;
    if (node->numChildren == 0) {
        node->children[0] = nodeToMove;
        node->numChildren++;
        return;
    }
    moveNode(nodeToMove, node->children[0]);
}

int main(int argc, char *argv[]) {
    //Read from file - Trapani Andrea (same as other version)
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Time measurement - Vavassori Enrico (same as other version)
    clock_t start, end;
    start = clock();

    int numNodes = 0;
    Node *root = createTree(file, &numNodes);
    fclose(file);

    if (root != NULL) {
        //printNodes(root);
        int cost = 0;
        
        //done by all
        while (isToDetach(root)) {
            Node* nodeToDetach = findNodeToDetach(root);
            moveNode(nodeToDetach, root);
            cost += nodeToDetach->value;
        }
        //printf("\n\n --- MOVED NODES --- \n\n");
        //printNodes(root);
        printf("TOTAL COST = %d\n", cost);
        freeTree(root);
    }

    end = clock();

    // Calculate execution time in seconds
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time: %.5f sec\n", time_taken);

    return 0;
}