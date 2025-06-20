#include "viz.h"

int adjMatrix[MATRIX_SIZE * MATRIX_SIZE] = {0};
int node_colors[MATRIX_SIZE] = {0};
int in_degrees[MATRIX_SIZE] = {0};
int out_degrees[MATRIX_SIZE] = {0};
int edge_strengths[MATRIX_SIZE] = {0};

char msg_buffer[128];

int active_node_count=0;

int* get_adjMatrix() {
    return adjMatrix;
}
int* get_node_colors() {
    return node_colors;
}
int* get_in_degrees() {
    return in_degrees;
}
int* get_out_degrees() {
    return out_degrees;
}
int* get_edge_strengths(){
    return edge_strengths;
}

void fill_adjMatrix(int* smaller, int n){
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            adjMatrix[i * MATRIX_SIZE + j] = smaller[i*n+j];
        }
    }
}

void remove_node(int* adjMatrix, int active_node_count_, int node_index) {
    sprintf(msg_buffer, "removeNode: %d\n", node_index);
    jsLog(msg_buffer);

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

    sprintf(msg_buffer, "active_node_count: %d\n", active_node_count);
    jsLog(msg_buffer);
    // Decrease active node count
    active_node_count--;
    sprintf(msg_buffer, "active_node_count: %d\n", active_node_count);
    jsLog(msg_buffer);

    for (int i = 0; i < active_node_count; i++) {
            adjMatrix[i * MATRIX_SIZE + active_node_count] = 0;
            adjMatrix[active_node_count * MATRIX_SIZE + i] = 0;
    }

    calculate_out_degrees(adjMatrix);
    calculate_in_degrees(adjMatrix);
}

void merge_nodes(int* adjMatrix, int active_node_count, int node1, int node2) {
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

    remove_node(adjMatrix, active_node_count, node2);
}

int get_active_node_count() {
    return active_node_count;
}

int calculate_node_count(int* adjMatrix) {
    if (adjMatrix == NULL) {
        sprintf(msg_buffer, "Error: Invalid adjacency matrix pointer");
        jsLog(msg_buffer);
        return 0;
    }

    int count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        int has_edges = 0;
        // Check both incoming and outgoing edges
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (adjMatrix[i * MATRIX_SIZE + j] > 0 || adjMatrix[j * MATRIX_SIZE + i] > 0) {
                has_edges = 1;
                break;
            }
        }
        if (has_edges) {
            count++;
        }
    }
    
    snprintf(msg_buffer, sizeof(msg_buffer), "node count: %d\n", count);
    jsLog(msg_buffer);
    return count;
}

int node_out_degree(int* adjMatrix, int node_index) {
    int degree = 0;

    for(int i=0; i < MATRIX_SIZE; i++) {
        degree += adjMatrix[node_index * MATRIX_SIZE + i];
    }

    // sprintf(msg_buffer, "node %d out degree: %d\n", node_index, degree);
    // jsLog(msg_buffer);
    return degree;
}

void calculate_out_degrees(int* adjMatrix) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        out_degrees[i] = node_out_degree(adjMatrix, i);
    }
}

int node_in_degree(int* adjMatrix, int node_index) {
    int degree = 0;

    for(int i=0; i < MATRIX_SIZE; i++) {
        degree += adjMatrix[i * MATRIX_SIZE + node_index];
    }

    // sprintf(msg_buffer, "node %d in degree: %d\n", node_index, degree);
    // jsLog(msg_buffer);
    return degree;
}

void calculate_in_degrees(int* adjMatrix) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        in_degrees[i] = node_in_degree(adjMatrix, i);
    }
}
