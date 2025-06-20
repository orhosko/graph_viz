#include "viz.h"

int n = 4;

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
    active_node_count = 16;
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

int print_active_node_count() {
    sprintf(msg_buffer, "active_node_count: %d\n", active_node_count);
    jsLog(msg_buffer);
    return active_node_count;
}

void step(int* adjMatrix) {
    active_node_count = calculate_node_count(adjMatrix);

    if (active_node_count <= 1) return;  // Safety check

    int node1 = rand() % active_node_count;
    int node2 = rand() % active_node_count;
    sprintf(msg_buffer, "node1: %d, node2: %d\n", node1, node2);
    jsLog(msg_buffer);
    merge_nodes(adjMatrix, active_node_count, node1, node2);

    updateGraph();
}