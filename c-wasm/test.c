extern void jsLog(char* msg);
extern void updateGraph();
extern int rand();

char msg_buffer[128];

int* get_msg_buffer() {
    return (int*)msg_buffer;
}

#define MATRIX_SIZE 32
int adjMatrix[MATRIX_SIZE * MATRIX_SIZE] = {0};

int* get_adjMatrix() {
    return adjMatrix;
}

int node_colors[MATRIX_SIZE] = {0};

int* get_node_colors() {
    return node_colors;
}

void fill_msg_buffer(){
    for (int i = 0; i < 16; i++) {
        msg_buffer[i] = '1';
    }
}

int active_node_count = MATRIX_SIZE;  // Track active nodes separately from matrix size

void create_row_deps(int* adjMatrix, int n) {
    for (int i = 0; i < active_node_count; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i % n != j) {
                adjMatrix[(i * MATRIX_SIZE) + ((i / n) * n) + j] += 1;
            }
        }
    }
}

void create_col_deps(int* adjMatrix, int n) {
    for (int i = 0; i < active_node_count; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i / n != j) {
                adjMatrix[(i * MATRIX_SIZE) + (i % n) + (j * n)] += 1;
            }
        }
    }
}

void create_block_deps(int* adjMatrix, int n) {
    for (int i = 0; i < active_node_count; ++i) {
        int x = i % n;
        int y = i / n;
        int block_index = ((y / n) * n) + (x / n);

        int block_min = ((block_index % n) * n) + ((block_index / n) * n*n);

        adjMatrix[(i * MATRIX_SIZE) + block_min] += 1;
        adjMatrix[(i * MATRIX_SIZE) + block_min + 1] += 1;
        adjMatrix[(i * MATRIX_SIZE) + block_min + 2] += 1;
        adjMatrix[(i * MATRIX_SIZE) + block_min + 3] += 1;
        adjMatrix[(i * MATRIX_SIZE) + block_min + n] += 1;
        adjMatrix[(i * MATRIX_SIZE) + block_min + n+1] += 1;
        adjMatrix[(i * MATRIX_SIZE) + block_min + n+2] += 1;
        adjMatrix[(i * MATRIX_SIZE) + block_min + n+3] += 1;

        adjMatrix[(i * MATRIX_SIZE) + i] -= 1;
    }
}

void normalize_adj_matrix(int* adjMatrix) {
    for (int i = 0; i < active_node_count; ++i) {
        for (int j = 0; j < active_node_count; ++j) {
          adjMatrix[(i * MATRIX_SIZE) + j] = (adjMatrix[(i * MATRIX_SIZE) + j] > 1) ? 1 : adjMatrix[(i * MATRIX_SIZE) + j];
        }
    }
}

void m_strncpy(char* dest, const char* src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
    dest[n] = '\0';
}

char msg2[] = "init";

int* get_msg2() {
    return (int*)msg2;
}

void init(int* adjMatrix) {
    jsLog("init");

    m_strncpy(msg_buffer, "init", 4);
    jsLog(msg_buffer);

    char msg1[] = "Hello from C";
    m_strncpy(msg_buffer, msg1, 12);
    jsLog(msg_buffer);

    m_strncpy(msg_buffer, msg2, 4);
    jsLog(msg_buffer);

    create_row_deps(adjMatrix, 4);
    create_col_deps(adjMatrix, 4);
    //create_block_deps(adjMatrix, 4);
    //normalize_adj_matrix(adjMatrix);

    // Assign colors based on row and column position
    for (int i = 0; i < active_node_count; ++i) {
        node_colors[i] = (i % 8) + 1;
    }
}

void run(int* adjMatrix) {
    // apply karger algorithm

}

void removeNode(int* adjMatrix, int node) {
    if (node >= active_node_count) return;  // Safety check

    for (int i = 0; i < MATRIX_SIZE; i++) {
        adjMatrix[node*MATRIX_SIZE + i] = 0;
        adjMatrix[i*MATRIX_SIZE + node] = 0;
    }

    // reorder the adjacency matrix
    for (int i = node; i < active_node_count - 1; i++) {
        for (int j = 0; j < active_node_count; j++) {
            adjMatrix[j*MATRIX_SIZE + i] = adjMatrix[j*MATRIX_SIZE + (i+1)]; // shift up
        }
    }
    for (int i = 0; i < active_node_count; i++) {
        for (int j = node; j < active_node_count - 1; j++) {
            adjMatrix[i*MATRIX_SIZE + j] = adjMatrix[i*MATRIX_SIZE + (j+1)]; // shift right
        }
    }

    active_node_count--;
}

void merge_nodes(int* adjMatrix, int node1, int node2) {
    if (node1 >= active_node_count || node2 >= active_node_count) return;  // Safety check

    // merge node2 into node1 by combining their edges
    for (int i = 0; i < active_node_count; i++) {
        if (adjMatrix[node2 * MATRIX_SIZE + i] > 0 || adjMatrix[node1 * MATRIX_SIZE + i] > 0) {
            adjMatrix[node1 * MATRIX_SIZE + i] = 1;
            adjMatrix[i * MATRIX_SIZE + node1] = 1; // Make it symmetric
        }
    }

    // Remove self-loops
    adjMatrix[node1 * MATRIX_SIZE + node1] = 0;

    // remove node2 from the adjacency matrix
    for (int i = 0; i < active_node_count; i++) {
        adjMatrix[node2*MATRIX_SIZE + i] = 0;
        adjMatrix[i*MATRIX_SIZE + node2] = 0;
    }

    removeNode(adjMatrix, node2);

    // update the graph visualization
    updateGraph();
}

void step(int* adjMatrix) {
    jsLog("step");

    m_strncpy(msg_buffer, "step", 4);
    jsLog(msg_buffer);



    fill_msg_buffer();
    jsLog(msg_buffer);

    char msg1[] = "Hello from C";
    m_strncpy(msg_buffer, msg1, 12);
    jsLog(msg_buffer);
    /*
    if (active_node_count <= 1) return;  // Safety check

    removeNode(adjMatrix, 0);
    updateGraph();
    return;
    // merge
    int node1 = rand() % active_node_count;
    int node2 = rand() % active_node_count;
    merge_nodes(adjMatrix, node1, node2);

    updateGraph();
    */
}

int add(int a, int b) {
    return a + b;
}