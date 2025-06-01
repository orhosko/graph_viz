extern void jsLog(char* msg, int len);
extern void updateGraph();
extern int rand();

int node_count = 16;

void create_row_deps(int* adjMatrix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i % 4 != j) {
                adjMatrix[(i * 16) + ((i / 4) * 4) + j] += 1;
            }
        }
    }
}

void create_col_deps(int* adjMatrix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i / 4 != j) {
                adjMatrix[(i * 16) + (i % 4) + (j * 4)] += 1;
            }
        }
    }
    jsLog("Hello from C", 12);
}

void create_block_deps(int* adjMatrix) {
    for (int i = 0; i < 16; ++i) {
        int x = i % 4;
        int y = i / 4;
        int block_index = ((y / 2) * 2) + (x / 2);

        int block_min = ((block_index % 2) * 2) + ((block_index / 2) * 8);

        adjMatrix[(i * 16) + block_min] += 1;
        adjMatrix[(i * 16) + block_min + 1] += 1;
        adjMatrix[(i * 16) + block_min + 4] += 1;
        adjMatrix[(i * 16) + block_min + 5] += 1;

        adjMatrix[(i * 16) + i] -= 1;
    }
}

void normalize_adj_matrix(int* adjMatrix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
          adjMatrix[(i * 16) + j] = (adjMatrix[(i * 16) + j] > 1) ? 1 : adjMatrix[(i * 16) + j];
        }
    }
}

// Color array to store node colors
int node_colors[16] = {0}; // Initialize all to 0 (no color)

void init(int* adjMatrix) {
    create_row_deps(adjMatrix);
    create_col_deps(adjMatrix);
    create_block_deps(adjMatrix);
    normalize_adj_matrix(adjMatrix);

    // Assign colors based on row and column position
    for (int i = 0; i < 16; ++i) {
        node_colors[i] = (i % 4) + 1;
    }
}

void run(int* adjMatrix) {
    // apply karger algorithm

}

void removeNode(int* adjMatrix, int node) {
    for (int i = 0; i < 16; i++) {
        adjMatrix[node*16 + i] = 0;
        adjMatrix[i*16 + node] = 0;
    }

    // reorder the adjacency matrix
    for (int i = node; i < node_count - 1; i++) {
        for (int j = 0; j < node_count; j++) {
            adjMatrix[j*16 + i] = adjMatrix[j*16 + (i+1)]; // shift up
        }
    }
    for (int i = 0; i < node_count; i++) {
        for (int j = node; j < node_count - 1; j++) {
            adjMatrix[i*16 + j] = adjMatrix[i*16 + (j+1)]; // shift right
        }
    }

    node_count--;
}

void merge_nodes(int* adjMatrix, int node1, int node2) {
    // merge node2 into node1
    // update the adjacency matrix
    // update the graph visualization

    // merge node2 into node1 by combining their edges
    for (int i = 0; i < 16; i++) {
        if (adjMatrix[node2 * 16 + i] > 0 || adjMatrix[node1 * 16 + i] > 0) {
            adjMatrix[node1 * 16 + i] = 1;
            adjMatrix[i * 16 + node1] = 1; // Make it symmetric
        }
    }

    // Remove self-loops
    adjMatrix[node1 * 16 + node1] = 0;

    // remove node2 from the adjacency matrix
    for (int i = 0; i < 16; i++) {
        adjMatrix[node2*16 + i] = 0;
        adjMatrix[i*16 + node2] = 0;
    }

    removeNode(adjMatrix, node2);


    // update the graph visualization
    updateGraph();
}



void step(int* adjMatrix) {
    // apply karger algorithm

    removeNode(adjMatrix, 0);
    updateGraph();
    return;
    // merge
    int node1 = rand() % 16;
    int node2 = rand() % 16;
    merge_nodes(adjMatrix, node1, node2);

    updateGraph();
}

int add(int a, int b) {
    return a + b;
}