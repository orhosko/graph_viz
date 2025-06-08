#include <stdio.h>
#include <string.h>

extern void jsLog(char* msg);
extern void updateGraph();
extern int rand();

#define MATRIX_SIZE 64
int adjMatrix[MATRIX_SIZE * MATRIX_SIZE] = {0};

int* get_adjMatrix() {
    return adjMatrix;
}

int node_colors[MATRIX_SIZE] = {0};

int* get_node_colors() {
    return node_colors;
}

int n = 2;
int active_node_count = 2*2;  // Track active nodes separately from matrix size

int get_active_node_count() {
    return active_node_count;
}

char msg_buffer[128];

void create_row_deps(int* adjMatrix) {
    for (int i = 0; i < active_node_count; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i % n != j) {
                adjMatrix[(i * MATRIX_SIZE) + ((i / n) * n) + j] += 1;
            }
        }
    }
}

void create_col_deps(int* adjMatrix) {
    for (int i = 0; i < active_node_count; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i / n != j) {
                adjMatrix[(i * MATRIX_SIZE) + (i % n) + (j * n)] += 1;
            }
        }
    }
}

void create_block_deps(int* adjMatrix) {
    for (int i = 0; i < active_node_count; ++i) {
        int row = i / n;
        int col = i % n;
            int block_row = row / n;
            int block_col = col / n;
            for (int dr = 0; dr < n; ++dr) {
                for (int dc = 0; dc < n; ++dc) {
                    int r = block_row * n + dr;
                    int c = block_col * n + dc;
                    int j = r * n + c;
                    if (j != i) {
                        adjMatrix[(i * MATRIX_SIZE) + j] += 1;
                    }
                }
        }
    }
}

void normalize_adj_matrix(int* adjMatrix) {
    for (int i = 0; i < active_node_count; ++i) {
        for (int j = 0; j < active_node_count; ++j) {
          adjMatrix[(i * MATRIX_SIZE) + j] = (adjMatrix[(i * MATRIX_SIZE) + j] > 1) ? 1 : adjMatrix[(i * MATRIX_SIZE) + j];
        }
    }
}

void init(int* adjMatrix) {
    jsLog("init");

    strncpy(msg_buffer, "init", sizeof(msg_buffer));
    jsLog(msg_buffer);

    char msg1[] = "Hello from C";
    strncpy(msg_buffer, msg1, sizeof(msg_buffer));
    jsLog(msg_buffer);

    create_row_deps(adjMatrix);
    create_col_deps(adjMatrix);
    //create_block_deps(adjMatrix);
    normalize_adj_matrix(adjMatrix);

    // Assign colors based on row and column position
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        node_colors[i] = (i % 4) + 1;
    }
}

void run(int* adjMatrix) {
    // apply karger algorithm

}

void removeNode(int* adjMatrix, int node_index) {
    if (node_index < 0 || node_index >= active_node_count)
        return;

    // Shift rows up (row i <- row i+1)
    for (int i = node_index; i < active_node_count - 1; ++i) {
        for (int j = 0; j < active_node_count; ++j) {
            adjMatrix[i * MATRIX_SIZE + j] = adjMatrix[(i + 1) * MATRIX_SIZE + j];
        }
    }

    // Shift columns left (col j <- col j+1)
    for (int i = 0; i < active_node_count; ++i) {
        for (int j = node_index; j < active_node_count - 1; ++j) {
            adjMatrix[i * MATRIX_SIZE + j] = adjMatrix[i * MATRIX_SIZE + j + 1];
        }
    }

    for (int i = 0; i < active_node_count; i++) {
            adjMatrix[i * MATRIX_SIZE + active_node_count] = 0;
            adjMatrix[active_node_count * MATRIX_SIZE + i] = 0;
    }

    // Decrease active node count
    active_node_count--;
}

void merge_nodes(int* adjMatrix, int node1, int node2) {
    if (node1 >= active_node_count || node2 >= active_node_count) return;  // Safety check

    if(node1 == node2) return;
    if(active_node_count == 2) return;

    // merge node2 into node1 by combining their edges
    for (int i = 0; i < active_node_count; i++) {
        if (adjMatrix[node2 * MATRIX_SIZE + i] > 0 || adjMatrix[node1 * MATRIX_SIZE + i] > 0) {
            adjMatrix[node1 * MATRIX_SIZE + i] += adjMatrix[node2 * MATRIX_SIZE + i];
            adjMatrix[i * MATRIX_SIZE + node1] += adjMatrix[i * MATRIX_SIZE + node2]; // Make it symmetric
        }
    }

    // remove self-loop
    adjMatrix[node1 * MATRIX_SIZE + node1] = 0;

    removeNode(adjMatrix, node2);
}

void step(int* adjMatrix) {
    // sprintf(msg_buffer, "%p", adjMatrix);
    // jsLog(msg_buffer);

    if (active_node_count <= 1) return;  // Safety check

    int node1 = rand() % active_node_count;
    int node2 = rand() % active_node_count;
    sprintf(msg_buffer, "node1: %d, node2: %d\n", node1, node2);
    jsLog(msg_buffer);
    merge_nodes(adjMatrix, node1, node2);

    updateGraph();
}